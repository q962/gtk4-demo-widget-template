---@diagnostic disable: undefined-global, undefined-field
add_rules("mode.debug", "mode.release")

target("gtk4_demo1")
    set_kind("binary")
    set_pcheader("src/stdafx.h")
    set_runenv("PATH","$(GTK4_BIN_PATH)", os.getenv("PATH"));

    add_files("src/**.c")
    add_defines("UNICODE", "_UNICODE")

    on_load(function(target)
        import("net.http")
        import("core.project.config")
        config.load(".config")

        config.set("APPID", "io.github.q962." .. target:name());
        target:add("defines", vformat("APPID=\"$(APPID)\""));

        if not os.isfile("./.xmake/xmakefuns.lua") then
            http.download("https://cdn.jsdelivr.net/gh/q962/xmake_funs/xmake.funs.lua", ".xmake/xmakefuns.lua");
            if not os.isfile("./.xmake/xmakefuns.lua") then
                print("download fail");
                os.exit();
            end
        end

        import("xmakefuns", {alias = "lx", rootdir= ".xmake"});

        if not os.isdir(path.join(target:targetdir(), "res")) then
            local res_dir = path.join(path.relative(".", target:targetdir()), "res");
            os.cd(target:targetdir());
            os.ln(res_dir, "res");
            os.cd(os.projectdir());
        end

        os.addenv("PKG_CONFIG_PATH", config.get("GTK4_DEBUG_PKG_CONFIG_PATH"))

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
    end)

    after_build(function()
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
        target:add("runenv", vformat("$(APPID).RESPATH"), "res/");
    end)

    on_install(function(target)
        import("core.base.task")
        import("lib.detect.find_program")
        import("core.project.config")
        config.load();

        if target:installdir() == "" then
            print("fail: no installdir");
            os.exit();
        end

        local pkg_vars = config.get("pkg_vars") or {};
        if #pkg_vars == 0 then
            for packagename, vars in pairs({
                ["gio-2.0"] = {
                    "gdbus"
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

        local function find_dep(target)
            import("core.project.config")

            local outdlls = {};
            local function get_dll_deps(dllname)
                local deps_str = os.iorunv("ldd", { dllname });
                for _, v in ipairs(deps_str:split("\n")) do
                    local dll = v:split(" ")[3];
                    if dll and not outdlls[dll] and dll:match("^/[^c]") then
                        outdlls[dll] = true;
                        get_dll_deps(dll)
                    end
                end
            end

            get_dll_deps(target:targetfile());
            get_dll_deps(pkg_vars["gio-2.0"].gdbus .. (is_plat("mingw") and ".exe" or ""));

            return outdlls;
        end

        local deps = find_dep(target);

        for v, _ in pairs(deps) do
            if not os.isfile(path.join(target:installdir(), "bin", path.filename(v))) then
                os.cp(path.join(config.get("mingw"), "..", v), path.join(target:installdir(), "bin") .. "/");
            end
        end

        local function cp(a, b)
            print("copy", a);
            os.cp(a,b);
        end

        -- main programe
        cp(target:targetfile(), path.join(target:installdir(), "bin") .. "/");
        -- res
        cp(path.join(os.projectdir(), "res"), path.join(target:installdir(), "share", vformat("$(APPID)")) .. "/");
        -- gdbus
        cp(pkg_vars["gio-2.0"].gdbus .. (is_plat("mingw") and ".exe" or ""), path.join(target:installdir(), "bin") .. "/");
        -- gtk dep file
        cp(path.join(pkg_vars.gtk4.prefix, "share", "gtk-4.0"), path.join(target:installdir(), "share") .. "/");
        -- gdk dep file
        cp(
            path.join(pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_moduledir, is_plat("mingw") and "*.dll" or "*.so"),
            path.join(
                target:installdir(),
                path.relative(
                    pkg_vars["gdk-pixbuf-2.0"].gdk_pixbuf_moduledir,
                    pkg_vars["gdk-pixbuf-2.0"].prefix)
            ) .. "/"
        );
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

        -- 如果是安装程序，最好使用 lnk 文件，隐藏目录结构。
        -- win32 上在 <install> 目录下放置 lnk 文件
    end)
