#pragma once

#include "HttpResponse.h"
#include "HttpRequest.h"
#include "Router.h"

#include <filesystem>
#include <vector>

namespace Pascal 
{
    class StaticFileRouter : public Router
    {
    public:
        StaticFileRouter() { }
        virtual ~StaticFileRouter() override { }

        void AddPath(const std::filesystem::path& path) 
        {
            m_Paths.emplace_back(path);
        }

        virtual Shared<HttpResponse> Route(
                                        const Shared<HttpRequest>& request,
                                        RoutingError& error) override;

        void SetUseImplicitPage(bool useImplicitPage)
        { m_UseImplicitPage = useImplicitPage; }

        void SetImplicitPage(const std::string& implicitPage) 
        { m_ImplicitPage = implicitPage; }

    private:
        Shared<HttpResponse> CreateFileResponse(const std::filesystem::path& filePath);

        bool m_UseImplicitPage = true;
        std::string m_ImplicitPage = "index.html";
        std::vector<std::filesystem::path> m_Paths;
    };
}