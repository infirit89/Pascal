project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{prj.location}/bin/" .. outputdir)
    objdir ("%{prj.location}/bin-int/" .. outputdir)

    files 
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "%{wks.location}/PascalServer/src/",
        "%{wks.location}/PascalServer/vendor/spdlog/include/"
    }

    links 
    {
        "PascalServer"
    }

    defines 
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines "PS_DEBUG"
        runtime "Debug"
        symbols "on"


    filter "configurations:Release"
        defines "PS_RELEASE"
        runtime "Release"
        optimize "on"
