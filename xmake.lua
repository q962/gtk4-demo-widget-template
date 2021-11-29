set_project("gtk4_demo_widget_template")
set_xmakever("2.5.9")

add_rules("mode.debug", "mode.release")

target("gtk4_demo1")
    set_default(true)
    set_kind("binary")
    set_pcheader("src/stdafx.h")
    set_runenv("PATH","$(GTK4_BIN_PATH)", os.getenv("PATH"));

    add_files("src/**.c")
    add_defines("UNICODE", "_UNICODE")

    on_load(function(target)
        import("net.http")
        import("core.project.config")
        import("core.project.project")
        config.load(".config")

        config.set("APPID", "io.github.q962." .. project.name());
        target:add("defines", "APPID=\""..config.get("APPID").."\"");

        if not os.isfile("./.xmake/xmakefuns.lua") then
            http.download("https://cdn.jsdelivr.net/gh/q962/xmake_funs/xmake.funs.lua", ".xmake/xmakefuns.lua");
            if not os.isfile("./.xmake/xmakefuns.lua") then
                print("download fail");
                os.exit();
            end
        end

        import("xmakefuns", {alias = "lx", rootdir= ".xmake"});

        local PKG_CONFIG_PATH = config.get("PKG_CONFIG_PATH");
        if  PKG_CONFIG_PATH and #PKG_CONFIG_PATH > 0 then
            for i=1, #PKG_CONFIG_PATH do
                os.addenv("PKG_CONFIG_PATH", PKG_CONFIG_PATH[i])
            end
        end

        lx.need(target,
            {"pkgconfig::gtk4 >= 4.4.0"},
            {"*glib-compile-resources", "sassc", "stat", "*ldd", "*pkg-config"}
        );

        -- 无法判断文件是否被修改，如有必要，写成函数遍历资源文件和输出文件对比日期
        os.run("glib-compile-resources --generate-header --sourcedir res res/res.xml --target src/res.h")
        os.run("glib-compile-resources --generate-source --sourcedir res res/res.xml --target src/res.c")

        if is_host("windows") then
            target:add("links", "Gdi32") -- 加载字体函数
        else
            target:add("defines", format("DATA_PREFIX=\"%s\"", target:installdir()));
        end

        target:add("cflags", "-g3", "-gdwarf-2", "-Winvalid-pch");

        local path_prefix = vformat("$(buildir)/app_"..config.get("mode"));

        target:set("targetdir", path.join(path_prefix, "bin"));
        if not os.isdir(path.join(path_prefix, "bin")   ) then os.mkdir(path.join(path_prefix, "bin")  ); end
        if not os.isdir(path.join(path_prefix, "lib")   ) then os.mkdir(path.join(path_prefix, "lib")  ); end
        if not os.isdir(path.join(path_prefix, "share") ) then os.mkdir(path.join(path_prefix, "share")); end
        if not os.isdir(path.join(path_prefix, "share", config.get("APPID")) )
            then os.mkdir(path.join(path_prefix, "share", config.get("APPID")));
        end

        local res_dir_path = path.join(path_prefix, "share", config.get("APPID"));
        if not os.isdir(path.join(res_dir_path, "res")) then
            local res_dir = path.join(path.relative(os.projectdir(), res_dir_path), "res");
            os.cd(res_dir_path);
            try {
                function() os.ln(res_dir, "res"); end,
                catch {
                    function(errors)
                        print(errors)
                        cprint("${red}需要管理员权限，用于创建软连接");
                        os.exit();
                    end
                }
            }
            os.cd(os.projectdir());
        end
    end)

    after_build(function(target)
        import("xmakefuns", {alias = "lx", rootdir= ".xmake"});
        import("core.project.config")
        config.load()

        local is_ok = true;

        for _, file_path in ipairs(os.files("$(projectdir)/res/css/*.scss")) do
            local out_path = file_path:gsub("scss$","css");

            if config.get("has_sassc") then
                if not os.isfile(out_path) or lx.do_stat(out_path, file_path) then
                    os.run("sassc " .. file_path .. " " .. out_path)
                else
                    cprint("${red}需要编译 " .. path.relative(file_path) .. " >> " .. path.relative(out_path))
                    is_ok = false;
                end
            end
        end

        if not is_ok then
            os.exit();
        end
    end)

    before_run(function(target)
        import("core.project.config")
        config.load();

        if is_plat("linux") then
            target:set("runenv", config.get("APPID")..".DATAPAHT", "..");
        end
    end)

    on_install("@windows", function(target)
        import("core.base.task")
        import("lib.detect.find_program")
        import("core.project.config")
        config.load();

        if target:installdir() == "" then
            print("fail: no installdir");
            os.exit();
        end

        local dllsuffix = is_host("windows") and ".dll" or ".so";
        local exesuffix = is_host("windows") and ".exe" or "";

        local pkg_vars = config.get("pkg_vars") or {};
        if #pkg_vars == 0 then
            for packagename, vars in pairs({
                ["gio-2.0"] = {
                    "prefix",
                    "gdbus",
                    "schemasdir",
                    "giomoduledir"
                },
                gtk4 = {
                    "prefix"
                },
                ["gdk-pixbuf-2.0"] = {
                    "prefix",
                    "gdk_pixbuf_moduledir",
                    "gdk_pixbuf_cache_file",
                    "gdk_pixbuf_binarydir"
                }
            }) do
                pkg_vars[packagename] = pkg_vars[packagename] or {}
                for _, var in ipairs(vars) do
                    local s = try{ function() return os.iorunv("pkg-config", { "--variable", var, packagename }) end }
                    if s then
                        pkg_vars[packagename][var] = s:trim();
                    end
                end
            end
            config.set("pkg_vars", pkg_vars);
            config.save();
        end

        local outdlls = {};
        local function find_dep(dep_path)
            local dep_dlls = {};
            local function get_dll_deps(dllname)
                local deps_str = os.iorunv("ldd", { dllname });
                for _, v in ipairs(deps_str:split("\n")) do
                    local dll = v:split(" ")[3];
                    if dll and not outdlls[dll] and dll:match("^/[^c]") then
                        outdlls[dll] = true;
                        dep_dlls[dll] = true;
                        get_dll_deps(dll)
                    end
                end
            end

            get_dll_deps(dep_path);

            for v, _ in pairs(dep_dlls) do
                if not os.isfile(path.join(target:installdir(), "bin", path.filename(v))) then
                    if is_plat("mingw") then
                        os.cp(path.join(config.get("mingw"), "..", v), path.join(target:installdir(), "bin") .. "/");
                    else
                        os.cp(v, path.join(target:installdir(), "bin") .. "/");
                    end
                end
            end
            return dep_dlls;
        end

        find_dep(target:targetfile());
        find_dep(pkg_vars["gio-2.0"].gdbus .. exesuffix);

        local function cp(a, b)
            a = a:gsub("\\", "/");
            print("copy", a, "==>", b);
            os.cp(a,b);
        end

        -- main programe
        cp(target:targetfile(), path.join(target:installdir(), "bin") .. "/");
        -- res
        cp(path.join(os.projectdir(), "res"), path.join(target:installdir(), "share", vformat("$(APPID)")) .. "/");
        -- gdbus
        cp(pkg_vars["gio-2.0"].gdbus .. exesuffix, path.join(target:installdir(), "bin") .. "/");
        find_dep(pkg_vars["gio-2.0"].gdbus .. exesuffix)
        -- gtk dep file
        cp(path.join(pkg_vars.gtk4.prefix, "share", "gtk-4.0"), path.join(target:installdir(), "share") .. "/");
        -- gstreamer
        local module_gstreamer_path = path.join(pkg_vars.gtk4.prefix, "lib", "gtk-4.0", "4.0.0", "media", "libmedia-gstreamer" .. dllsuffix);
        cp(
            module_gstreamer_path,
            path.join(target:installdir(), "lib", "gtk-4.0", "4.0.0", "media", "libmedia-gstreamer") .. "/"
        );
        find_dep(module_gstreamer_path)
        -- gdk dep file
        cp(
            path.join(pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_moduledir, "*" .. dllsuffix),
            path.join(
                target:installdir(),
                path.relative(
                    pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_moduledir,
                    pkg_vars["gdk-pixbuf-2.0"].prefix)
            ) .. "/"
        );
        for _,v in ipairs(os.files(path.join(pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_moduledir, "*" .. dllsuffix))) do
            find_dep(v);
        end
        -- gdk dep file
        cp(
            pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_cache_file,
            path.join(
                target:installdir(),
                path.relative(
                    pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_binarydir,
                    pkg_vars["gdk-pixbuf-2.0"].prefix)
            ) .. "/"
        );
        -- mo file
        cp(
            path.join(pkg_vars["gdk-pixbuf-2.0"].prefix, "share", "locale", "zh_CN", "LC_MESSAGES", "gtk40.mo"),
            path.join(target:installdir(), "share", "locale", "zh_CN", "LC_MESSAGES") .. "/"
        );
        cp(
            path.join(pkg_vars["gdk-pixbuf-2.0"].prefix, "share", "locale", "zh_CN", "LC_MESSAGES", "gtk40-properties.mo"),
            path.join(target:installdir(), "share", "locale", "zh_CN", "LC_MESSAGES") .. "/"
        );
        -- cp glib dep file
        cp(
            path.join(pkg_vars["gio-2.0"].schemasdir, "gschemas.compiled"),
            path.join(
                target:installdir(),
                path.relative(
                    pkg_vars["gio-2.0"].schemasdir,
                    pkg_vars["gio-2.0"].prefix)
            ) .. "/"
        );
        -- cp gio dep file
        cp(
            pkg_vars["gio-2.0"].giomoduledir,
            path.join(
                target:installdir(),
                path.relative(
                    pkg_vars["gio-2.0"].giomoduledir,
                    pkg_vars["gio-2.0"].prefix),
                ".."
            ) .. "/"
        );
        for _,v in ipairs(os.files(path.join(pkg_vars["gio-2.0"].giomoduledir, "*" .. dllsuffix))) do
            find_dep(v);
        end
        -- -- cp icons
        -- if is_plat("mingw") then
        --     cp(
        --         path.join(config.get("mingw"), "share", "icons", "Adwaita"),
        --         path.join(target:installdir(), "share", "icons") .. "/"
        --     );
        --     cp(
        --         path.join(config.get("mingw"), "share", "icons", "hicolor"),
        --         path.join(target:installdir(), "share", "icons") .. "/"
        --     );
        -- else
        --     os.raise("需要手动指定 Adwaita\\hicolor 主题所在目录")
        -- end

        -- 如果是安装程序，最好使用 lnk 文件，隐藏目录结构。
        -- win32 上在 <install> 目录下放置 lnk 文件
    end)
    on_install("@linux", function(target)
        if target:installdir() == "" then
            print("fail: no installdir");
            os.exit();
        end

        local function cp(a, b)
            a = a:gsub("\\", "/");
            print("copy", a, "==>", b);
            os.cp(a,b);
        end

        -- main programe
        cp(target:targetfile(), path.join(target:installdir(), "bin") .. "/");
        -- res
        cp(path.join(os.projectdir(), "res"), path.join(target:installdir(), "share", vformat("$(APPID)")) .. "/");
    end)
