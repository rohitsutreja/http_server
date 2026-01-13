#include "http_server/Router.hpp"

#include <assert.h>

namespace http_server
{
    HttpResponse Router::route(HttpRequest &req)
    {
        const auto segments{details::split(req.url, '/')};
        auto curr{_root.get()};

        for (const auto &seg : segments)
        {
            // In case someone put // in path or split returned empty seg for first /.
            if (seg.empty())
            {
                continue;
            }

            // if we have this segment then move to that path.
            if (curr->children.contains(seg))
            {
                curr = curr->children[seg].get();
            }
            // if we dont have that exact match, maybe we have param.
            else if (curr->param_child)
            {
                req.params[curr->param_child->param_name] = seg;
                curr = curr->param_child.get();
            }
            // This means we do not have given path.
            else
            {
                return HttpResponse{.status_code = 404, .status_message = "Not Found"};
            }
        }

        // We reached the correct path node. Now check the method.
        if (curr->is_end)
        {
            if (curr->method_handlers.contains(req.method))
            {
                return curr->method_handlers[req.method](req);
            }
            else
            {
                // Path exists, but doesn't support this method!
                return HttpResponse{.status_code = 405, .status_message = "Method Not Allowed"};
            }
        }

        return HttpResponse{.status_code = 404, .status_message = "Not Found"};
    }

    void Router::add_route_handler(HttpMethod method, const std::string &path, const HttpHandler &handler)
    {
        auto segments = details::split(path, '/');
        auto curr = _root.get();

        for (const auto &seg : segments)
        {
            if (seg.empty())
            {
                continue;
            }

            // if this segment is param than this is special match all segment.
            if (seg[0] == ':')
            {
                // We will override if user register param at same position in path. Means /users/:id will overwrite /users/:name
                if (!curr->param_child)
                {
                    curr->param_child = std::make_unique<details::TrieNode>();
                    curr->param_child->param_name = seg.substr(1);
                }
                curr = curr->param_child.get();
            }
            else
            {
                if (!curr->children.contains(seg))
                {
                    curr->children[seg] = std::make_unique<details::TrieNode>();
                }
                curr = curr->children[seg].get();
            }
        }

        curr->is_end = true;
        curr->method_handlers[method] = handler;
    }
}