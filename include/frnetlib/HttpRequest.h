//
// Created by fred on 10/12/16.
//

#ifndef FRNETLIB_HTTPREQUEST_H
#define FRNETLIB_HTTPREQUEST_H
#include <string>
#include <vector>
#include <unordered_map>
#include "TcpSocket.h"
#include "Http.h"

namespace fr
{
    class HttpRequest : public Http
    {
    public:
        //Constructors
        HttpRequest();
        HttpRequest(HttpRequest &&other)=default;
        void operator=(const HttpRequest &other)
        {
            header_ended = other.header_ended;
            last_parsed_character = other.last_parsed_character;
            content_length = other.content_length;
        }
        virtual ~HttpRequest() = default;

        /*!
         * Parse a HTTP response.
         *
         * @param data The HTTP response to parse
         * @param datasz The length of data in bytes
         * @return True if more data is needed, false if finished.
         */
        bool parse(const char *data, size_t datasz) override;

        /*!
         * Constructs a Http Request, ready to send.
         *
         * @return The constructed HTTP request.
         */
        std::string construct(const std::string &host) const override;

    private:
        /*!
         * Parses the request header.
         *
         * @param header_end_pos The position in 'body' of the end of the header
         */
        bool parse_header(int64_t header_end_pos);

        /*!
         * Parses the POST data from the body
         */
        void parse_post_body();

        /*!
         * Parses the header type (GET/POST) from the given string.
         *
         * @param str The first header line
         */
        void parse_header_type(const std::string &str);

        /*!
         * Parses the header URI
         *
         * @param str The first header line
         */
        void parse_header_uri(const std::string &str);

        //State
        bool header_ended;
		int32_t last_parsed_character;
        size_t content_length;

    };
}


#endif //FRNETLIB_HTTPREQUEST_H
