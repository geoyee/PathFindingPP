set_project("PathFinding")
set_version("0.0.1")

set_languages("c++14")
set_optimize("fastest")

add_rules("mode.debug", "mode.release")

option("test", {
    default = is_config("test", nil) and true or false,
    description = "Build tests",
})

option("shared", {
    default = false,
    description = "Build shared library",
})

option("gui", {
    default = false,
    description = "Build GUI application",
})

target("PathFinding", function()
    if has_config("shared") then
        set_kind("shared")
        add_defines("PATHFINDING_BUILDING_DLL", { public = false })
    else
        set_kind("static")
    end
    add_files("src/**.cpp")
    add_headerfiles("include/(**.h)", "include/(**.hpp)")
    add_includedirs("include", { public = true })
    if is_plat("windows") then
        add_cxxflags("/utf-8", "/W4", "/EHsc", "/Zc:__cplusplus", "/Zc:preprocessor", "/Gy", { force = true })
        if is_mode("release") then
            add_cxxflags("/O2", "/GL", { force = true })
            add_ldflags("/LTCG", "/OPT:REF", "/OPT:ICF", { force = true })
        else
            add_cxxflags("/Od", { force = true })
        end
    else
        add_cxxflags(
            "-fPIC",
            "-Wall",
            "-Wextra",
            "-Wconversion",
            "-Wsign-compare",
            "-Werror=uninitialized",
            "-Werror=return-type",
            "-Werror=unused-result",
            "-Werror=suggest-override",
            "-Wzero-as-null-pointer-constant",
            "-Wmissing-declarations",
            "-Wold-style-cast",
            "-Wnon-virtual-dtor",
            { force = true }
        )
        if is_mode("release") then
            add_cxxflags("-O3", "-flto", { force = true })
            add_ldflags("-flto", { force = true })
        else
            add_cxxflags("-g", { force = true })
        end
    end
end)

if has_config("test") then
    add_requires("catch2")

    target("PathFinding_test", function()
        set_kind("binary")
        add_files("test/test_main.cpp")
        add_deps("PathFinding")
        add_packages("catch2")
    end)
end

if has_config("gui") then
    add_requires("qt6")

    target("PathFinding_visual", function()
        set_kind("binary")
        add_files("visual/src/*.cpp", "visual/src/*.h", "visual/resources.qrc")
        add_files("visual/qml/*.qml", { rules = "qt.qrc" })
        add_deps("PathFinding")
        add_packages("qt6")
        add_includedirs("include", "visual/src")
        add_defines("QT_NO_KEYWORDS")
        add_rules("qt.moc", "qt.qrc")
    end)
end
