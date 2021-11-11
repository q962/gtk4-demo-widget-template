add_rules("mode.debug", "mode.release")

target("gtk4_demo1")
    set_kind("binary")
    set_pcheader("src/stdafx.h")
    set_rundir(".") -- 资源目录
    set_runenv("PATH","$(GTK4_BIN_PATH)", os.getenv("PATH"));

    add_files("src/**.c")
    add_files("res/res.c")

    add_includedirs("res") -- 资源文件
    add_links("Gdi32") -- 加载字体

    add_defines("UNICODE", "_UNICODE")

    before_build(function(target)
        import("lib.detect.find_program")

        if not find_program("glib-compile-resources") then
            print("缺少 glib-compile-resources 程序")
            -- 输出个屁，如何优雅的终止程序，而不输出调用栈
        end
    end)

    on_load(function(target)
        import("core.project.config")
        config.load(".config");

        -- 无法判断文件是否被修改，如有必要，写成函数遍历资源文件和输出文件对比日期
        os.run("glib-compile-resources --generate-header --sourcedir res res/res.xml")
        os.run("glib-compile-resources --generate-source --sourcedir res res/res.xml")

        os.addenv("PKG_CONFIG_PATH", config.get("GTK4_DEBUG_PKG_CONFIG_PATH") or "")

        target:add(
            find_packages(
                "pkgconfig::gtk4 >= 4.4.0"
            )
        );

    end)
