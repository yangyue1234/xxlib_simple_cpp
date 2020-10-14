#pragma once

#include <vector>
#if defined(__clang__) || defined(_MSC_VER)
#include <experimental/coroutine>
#define CorNamespace std::experimental
#else
#include <coroutine>
#define CorNamespace std
#endif

#define CorYield co_await CorNamespace::suspend_always{};
#define CorAwait(func) {auto g = func; while(g.Next()) {CorYield;}}
#define CorRtv xx::Generator<int>

namespace xx {
    using namespace CorNamespace;

    template<typename T>
    struct Generator {
        struct promise_type;

        using Handle = coroutine_handle<promise_type>;
    private:
        Handle h;
    public:
        explicit Generator(Handle &&h) : h(std::move(h)) {}

        Generator(const Generator &) = delete;

        Generator &operator=(Generator const &) = delete;

        Generator(Generator &&o) noexcept: h(o.h) {
            o.h = nullptr;
        }

        Generator &operator=(Generator &&o) noexcept {
            std::swap(h, o.h);
            return *this;
        }

        ~Generator() {
            if (h) {
                h.destroy();
            }
        }

        bool Next() {
            h.resume();
            return not h.done();
        }

        T Value() {
            return h.promise().v;
        }


        struct promise_type {
        private:
            T v{};

            friend class Generator;

        public:
            promise_type() = default;

            ~promise_type() = default;

            promise_type(promise_type const &) = delete;

            promise_type(promise_type &&) = delete;

            promise_type &operator=(promise_type const &) = delete;

            promise_type &operator=(promise_type &&) = delete;

            auto initial_suspend() {
                return suspend_always{};
            }

            auto final_suspend() {
                return suspend_always{};
            }

            auto get_return_object() {
                return Generator{Handle::from_promise(*this)};
            }

            auto return_void() {
                return suspend_never{};
            }

            auto yield_value(T some_value) {
                v = some_value;
                return suspend_always{};
            }

            void unhandled_exception() {
                std::exit(1);
            }
        };


        struct iterator_end {
        };

        struct iterator {
            template<class>
            friend
            class Generator;

            using iterator_category = std::input_iterator_tag;
            using value_type = T;

            T const &operator*() const {
                return p->v;
            }

            T &operator*() {
                return p->v;
            }

            void operator++() {
                Handle::from_promise(*p).resume();
            }

            bool operator!=(iterator_end) {
                return !Handle::from_promise(*p).done();
            }

        private:
            explicit iterator(promise_type * const& p) : p(p) {}

            promise_type *p;
        };

        iterator begin() { return iterator(&h.promise()); }

        iterator_end end() { return {}; }
    };



    struct Cors {
        std::vector<CorRtv > cs;

        inline bool Empty() {
            return cs.empty();
        }

        inline void Add(CorRtv &&c) {
            cs.emplace_back(std::move(c));
        }

        inline void Update() {
            // 倒扫以方便交换删除
            for (auto &&i = cs.size() - 1; i != (size_t) -1; --i) {
                if (!cs[i].Next()) {
                    if (i < cs.size() - 1) {
                        std::swap(cs[cs.size() - 1], cs[i]);
                    }
                    cs.pop_back();
                }
            }
        }
    };
}
