set_xmakever("2.5.9")

set_project("gtk4_demo_widget_template")
set_version("0.0.2")
set_config("APPID", "io.github.q962.gtk4_demo_widget_template")

add_rules("mode.debug", "mode.release")

target("gtk4_demo1")
    set_default(true)
    set_kind("binary")
    set_pcheader("src/stdafx.h")
    set_runenv("PATH","$(GTK4_BIN_PATH)", os.getenv("PATH"));

    add_files("src/**.c")
    add_defines("UNICODE", "_UNICODE")

    add_installfiles("res/**", {prefixdir="share/$(APPID)/"})

    -- 这应该由 xmake 添加，且应该删除 $ORIGIN/../../<plat>/<arch>/<mode>
    -- 如果有依赖需要手动添加
    -- add_rpathdirs("@loader_path/../lib")

    on_load(function(target)
        import("net.http")
        import("core.project.config")
        config.load(".xmake.config")

        -- 缺陷：绝对路径应该由 xmake 执行
        target:set("installdir", path.absolute(target:installdir() or "build/packing"))

        if not os.isfile("./.xmake/xmakefuns.lua") then
            http.download("https://cdn.jsdelivr.net/gh/q962/xmake_funs/xmakefuns.lua", ".xmake/xmakefuns.lua");
            if not os.isfile("./.xmake/xmakefuns.lua") then
                print("download fail");
                os.exit();
            end
        end

        import("xmakefuns", {alias = "lx", rootdir= ".xmake"});

        lx();

        lx.need(target,
            {"pkgconfig::gtk4 >= 4.4.0", "pkgconfig::fontconfig"},
            {"*glib-compile-resources", "sassc", "stat", "*ldd", "*pkg-config"}
        );
    end)

    before_build(function(target)
        import("core.project.config")
        config.load()

        -- 无法判断文件是否被修改，如有必要，写成函数遍历资源文件和输出文件对比日期
        os.exec("glib-compile-resources --generate-header --sourcedir res res/res.xml --target src/res.h")
        os.exec("glib-compile-resources --generate-source --sourcedir res res/res.xml --target src/res.c")

        target:add("cflags", "-g3", "-gdwarf-2", "-Winvalid-pch");
        target:add("defines", format('APPID="%s"', config.get("APPID")));

        if is_host("windows") then
            target:add("links", "Gdi32") -- 加载字体函数
        else
            target:add("defines", format("DATA_PREFIX=\"%s\"", target:installdir()));
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

        target:set("runenv", config.get("APPID")..".DATAPATH", "$(projectdir)");
    end)

    after_install("mingw", function(target)
        import("xmakefuns", {alias = "lx", rootdir= ".xmake"});

        lx.downfile("pack_gtk4")
        import("pack_gtk4", {alias = "lx", rootdir= ".xmake"});

        lx.pack_gtk4(target)
    end)
