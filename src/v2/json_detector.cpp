#include "json_detector.h"
#include "json_detector_p.h"

#include <cerrno>
#include <string_view>
#include <system_error>

namespace psb::inline json_detector_v2 {

json_detector::json_detector() : d_ptr(std::make_shared<json_detector_private>()) {}
json_detector::~json_detector() = default;

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
wwa::coro::generator<std::string> json_detector::process(std::string_view input, std::string_view::size_type max_size)
{
    auto impl = this->d_ptr;

    const std::string_view::size_type size = input.size();
    std::string_view::size_type copy_start = 0;
    std::string_view::size_type copy_count = 0;

    for (std::string_view::size_type pos = 0; pos < size; ++pos) {
        const auto c = input[pos];
        if (impl->buf.size() + copy_count >= max_size) [[unlikely]] {
            impl->buf.append(input.substr(copy_start, copy_count));
            throw std::system_error{EMSGSIZE, std::generic_category()};
        }

        ++copy_count;
        if (c == impl->open) {
            ++impl->count;
        }
        else if (c == impl->close) {
            --impl->count;
            if (impl->count == 0) {
                if (impl->buf.empty()) {
                    co_yield std::string(&input[copy_start], copy_count);
                }
                else {
                    impl->buf.append(input.substr(copy_start, copy_count));
                    co_yield impl->buf;
                    impl->buf.clear();
                }

                impl->open = 0;
                copy_count = 0;
                copy_start = std::string_view::npos;
            }
        }
        else if (c == '"') {
            if (impl->count == 0) [[unlikely]] {
                impl->buf.append(1, c);
                throw std::system_error{EBADMSG, std::generic_category()};
            }

            if (!impl->in_string) {
                std::string_view::size_type closing = pos;
                do {
                    closing = input.find('"', closing + 1);
                    if (closing == std::string_view::npos) {
                        impl->buf.append(input.substr(copy_start));
                        impl->escape    = input.back() == '\\';
                        impl->in_string = true;
                        co_return;
                    }
                } while (input[closing - 1] == '\\');
                copy_count += closing - pos;
                pos         = closing;
            }
            else if (!impl->escape) {
                impl->in_string = false;
            }
            else {
                impl->escape = false;
            }
        }
        else if (c == '\\' && impl->in_string) {
            impl->escape = !impl->escape;
        }
        else {
            if (impl->count == 0) {
                if (c == '{' || c == '[') {
                    impl->open  = c;
                    impl->close = static_cast<char>(c + 2);
                    impl->count = 1;
                    copy_start  = pos;
                    copy_count  = 1;
                }
                else if (std::isspace(c) == 0) [[unlikely]] {
                    impl->buf.append(input.substr(copy_start, copy_count));
                    throw std::system_error{EBADMSG, std::generic_category()};
                }
                else {
                    copy_start = std::string_view::npos;
                }
            }

            impl->escape = false;
        }
    }

    if (copy_start != std::string_view::npos && copy_count != 0) {
        impl->buf.append(input.substr(copy_start));
    }
}

bool json_detector::is_finished() const noexcept
{
    return this->d_ptr->count == 0 && !this->d_ptr->in_string;
}

const std::string& json_detector::get_json() const noexcept
{
    return this->d_ptr->buf;
}

void json_detector::reset() noexcept
{
    this->d_ptr->buf.clear();
    this->d_ptr->count     = 0;
    this->d_ptr->in_string = false;
    this->d_ptr->escape    = false;
    this->d_ptr->open      = 0;
    this->d_ptr->close     = 0;
}

}  // namespace psb::inline json_detector_v2
