#include "pspch.h"

#include "HttpResponseBuilder.h"
#include "Core/Buffer.h"

#include <spdlog/fmt/fmt.h>

namespace Pascal 
{
    std::string HttpResponseBuilder::BuildResponse(
                                                const Shared<HttpResponse>& response) 
    {
        Buffer buffer(PS_HTTP_REQUEST_INITIAL_SIZE);
        
        // write the status code to the response buffer
        buffer.WriteFormatted("HTTP/1.1 {0} {1}\r\n", 
                            (int)response->m_Status,
                            HttpStatusToString(response->m_Status));

        // write the content length
        buffer.WriteFormatted(
                            "Content-Length: {0}\r\n",
                            response->GetContentLength());

        // write all the headers
        for(const auto& [header, headerData] : response->GetHeaders())
            buffer.WriteFormatted("{0}: {1}\r\n", header, headerData);

        buffer.Write("\r\n");

        if(response->GetBody().size() > 0)
            buffer.WriteFormatted("{0}\r\n", response->GetBody());

        return buffer.GetData();
    }
}