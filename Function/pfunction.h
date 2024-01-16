#ifndef FUNCTION_PFUNCTION_H
#define FUNCTION_PFUNCTION_H
namespace pmcguire {
    template<typename T>
    class Function;

    template<typename ReturnValue, typename ... Args>
    class Function<ReturnValue(Args...)> {
    private:
        struct ICallable {
            virtual ReturnValue operator()(Args...) = 0;
            virtual ~ICallable() = default;
        };

        template<typename T>
        struct CallableT : public ICallable {
            T m_func;

            ~CallableT() override = default;

            explicit CallableT(const T &t) : m_func(t) {}

            ReturnValue operator()(Args... args) override {
                return m_func(args...);
            }
        };


        ICallable *m_callable = nullptr;


    public:
        explicit Function() = default;

        template<typename T>
        explicit Function(T t) {
            m_callable = new CallableT<T>(t);
        }

        ~Function() {
            delete m_callable;
        }

        template<typename T>
        Function &operator=(T t) {
            m_callable = new CallableT<T>(t);
            return *this;
        }

        ReturnValue operator()(Args... args) {
            return (*m_callable)(args...);
        }

    };

    template<typename T>
    class StackFunction;

    template<typename ReturnValue, typename ... Args>
    class StackFunction<ReturnValue(Args...)> {
    private:
        struct ICallable {
            virtual ReturnValue operator()(Args...) = 0;
        };

        template<typename T>
        struct CallableT : public ICallable {
            T *m_func;

            ~CallableT() = default;

            explicit CallableT(T *t) {
                m_func = t;
            }

            ReturnValue operator()(Args... args) override {
                return (*m_func)(args...);
            }
        };


        ICallable *m_callable = nullptr;
    public:
        explicit StackFunction() = default;

        template<typename T>
        explicit StackFunction(T *t) {
            m_callable = new CallableT<T>(t);
        }

        ~StackFunction() = default;

        template<typename T>
        StackFunction &operator=(T *t) {
            m_callable = new CallableT<T>(t);
            return *this;
        }

        const ReturnValue operator()(Args... args) {
            return (*m_callable)(args...);
        }

    };

}
#endif //FUNCTION_PFUNCTION_H
