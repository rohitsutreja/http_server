#include "http_server/App.hpp"

namespace http_server
{

    void App::use(const Middleware &middleware)
    {
        _middlewares.push_back(middleware);
    }

    HttpResponse App::_dispatch(int index, HttpRequest &req, NextFunction finalHandler)
    {

        if (index >= static_cast<int>(_middlewares.size()))
        {
            return finalHandler(req);
        }

        NextFunction next = [this, index, &finalHandler](HttpRequest &r)
        {
            return this->_dispatch(index + 1, r, finalHandler);
        };

        return _middlewares[index](req, next);
    }

    void App::get(const std::string &path, const HttpHandler &handler)
    {
        _router.add_route_handler(HttpMethod::GET, path, handler);
    }

    void App::post(const std::string &path, const HttpHandler &handler)
    {
        _router.add_route_handler(HttpMethod::POST, path, handler);
    }

    void App::put(const std::string &path, const HttpHandler &handler)
    {
        _router.add_route_handler(HttpMethod::PUT, path, handler);
    }

    void App::patch(const std::string &path, const HttpHandler &handler)
    {
        _router.add_route_handler(HttpMethod::PATCH, path, handler);
    }

    void App::delete_(const std::string &path, const HttpHandler &handler)
    {
        _router.add_route_handler(HttpMethod::DELETE, path, handler);
    }

    void App::start()
    {
       _server.set_handler([this](HttpRequest &req) -> HttpResponse
                           { 
                            return _dispatch(0, 
                                            req, 
                                            [this](HttpRequest &r)
                                            { 
                                                    return _router.route(r); 
                                            }
                                            ); 
                            });
        _server.run();
    }

}