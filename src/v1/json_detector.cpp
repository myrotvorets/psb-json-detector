#include "json_detector.h"
#include "json_detector_p.h"

#include <cerrno>
#include <string_view>
#include <system_error>

namespace psb::json_detector_v1 {

json_detector::json_detector() : d_ptr(std::make_shared<json_detector_private>()) {}
json_detector::~json_detector() = default;

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
wwa::coro::generator<std::string> json_detector::process(std::string_view input, std::string_view::size_type max_size)
{
    auto impl = this->d_ptr;

    for (const auto c : input) {
        if (impl->buf.size() >= max_size) {
            throw std::system_error{EMSGSIZE, std::generic_category()};
        }

        impl->buf.append(1, c);

        switch (c) {
            case '{':
                ++impl->brace_count;
                break;

            case '[':
                ++impl->bracket_count;
                break;

            case '}':
            case ']':
                if (c == '}') {
                    --impl->brace_count;
                }
                else {
                    --impl->bracket_count;
                }

                if (impl->brace_count < 0 || impl->bracket_count < 0) [[unlikely]] {
                    throw std::system_error{EBADMSG, std::generic_category()};
                }

                if (impl->brace_count == 0 && impl->bracket_count == 0) {
                    co_yield impl->buf;
                    impl->buf.clear();
                }

                break;

            case '"':
                if (impl->brace_count == 0 && impl->bracket_count == 0) [[unlikely]] {
                    throw std::system_error{EBADMSG, std::generic_category()};
                }

                if (!impl->in_string) {
                    impl->in_string = true;
                }
                else if (!impl->escape) {
                    impl->in_string = false;
                }
                else {
                    impl->escape = false;
                }

                break;

            case '\\':
                if (!impl->in_string) [[unlikely]] {
                    throw std::system_error{EBADMSG, std::generic_category()};
                }

                impl->escape = !impl->escape;
                break;

            default:
                if (impl->brace_count == 0 && impl->bracket_count == 0) {
                    if (std::isspace(c) != 0) [[likely]] {
                        impl->buf.pop_back();
                    }
                    else {
                        throw std::system_error{EBADMSG, std::generic_category()};
                    }
                }

                impl->escape = false;
                break;
        }
    }
}

bool json_detector::is_finished() const noexcept
{
    return this->d_ptr->brace_count == 0 && this->d_ptr->bracket_count == 0 && !this->d_ptr->in_string;
}

const std::string& json_detector::get_json() const noexcept
{
    return this->d_ptr->buf;
}

void json_detector::reset() noexcept
{
    this->d_ptr->buf.clear();
    this->d_ptr->brace_count   = 0;
    this->d_ptr->bracket_count = 0;
    this->d_ptr->in_string     = false;
    this->d_ptr->escape        = false;
}

}  // namespace psb::json_detector_v1
