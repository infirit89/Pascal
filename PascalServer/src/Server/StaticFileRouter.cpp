#include "pspch.h"
#include "StaticFileRouter.h"

#include <fstream>

namespace Pascal 
{
    Shared<HttpResponse> StaticFileRouter::Route(const Shared<HttpRequest>& request) 
    {
        // the only valid request method for static files should be get
        if(request->GetMethod() != HttpMethod::Get)
            return nullptr;

        for(const auto& directoryPath : m_Paths) 
        {
            std::string path = directoryPath.string() + request->GetTarget();
            if(std::filesystem::exists(path))
            {
                if(std::filesystem::is_directory(path)) 
                {
                    if(m_UseImplicitPage) 
                    {

                        return CreateFileResponse(directoryPath / m_ImplicitPage);
                    }
                    else 
                    {
                        // forbidden
                    }
                }
                else 
                {
                    return CreateFileResponse(path);
                }
            }
            else 
            {
                // not found
            }
        }

        return nullptr;
    }

    Shared<HttpResponse> StaticFileRouter::
                                        CreateFileResponse(
                                            const std::filesystem::path& filePath) 
    {
        Shared<HttpResponse> response = CreateShared<HttpResponse>();

        if(
            ContentTypeExtensionMap.find(
                filePath.extension()) == ContentTypeExtensionMap.end()) 
        {
            PS_ERROR("File type not supported: {}", filePath);
            return nullptr;
        }

        response->SetContentType(ContentTypeExtensionMap[filePath.extension()]);

        std::ifstream ifs(filePath, std::ifstream::in);

        if(!ifs) 
        {
            PS_ERROR("Can't open file: {}", filePath);
            return nullptr;
        }

        ifs.seekg(0, ifs.end);
        size_t fileSize = ifs.tellg();
        ifs.seekg(0, ifs.beg);

        char* fileData = new char[fileSize + 1];

        ifs.read(fileData, fileSize);

        if(!ifs)
        {
            PS_ERROR("Couldn't read from file: {}", filePath);
            return nullptr;
        }

        fileData[fileSize] = '\0';

        response->SetBody(fileData, fileSize);
        ifs.close();

        delete[] fileData;

        return response;
    }
}