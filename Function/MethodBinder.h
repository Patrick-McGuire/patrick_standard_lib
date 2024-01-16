namespace pmcguire {
    template<typename T>
    class Method;

    template<typename ReturnValue, typename ... Args>
    class Method<ReturnValue(Args...)> {
    public:
        struct ICallable {
            virtual ReturnValue call(Args...) = 0;

            virtual ~ICallable() = default;
        };

        template<typename T, typename F, typename C>
        struct ConstCallableT : public ICallable {
        public:
            const T &m_object;

            ReturnValue (C::* const methodPtr)(Args...) const;

            ~ConstCallableT() override = default;

            explicit ConstCallableT(const T &t, F f) : m_object(t), methodPtr(f) {}

            ReturnValue call(Args... args) override {
                return (m_object.*methodPtr)(args...);
            }
        };

        template<typename T, typename F, typename C>
        struct CallableT : public ICallable {
        public:
            T &m_object;

            ReturnValue (C::* methodPtr)(Args...);

            ~CallableT() override = default;

            explicit CallableT(T &t, F f) : m_object(t), methodPtr(f) {}

            ReturnValue call(Args... args) override {
                return (m_object.*methodPtr)(args...);
            }
        };

        template<typename T>
        struct OperatorCallableT : public ICallable {
        public:
            T &m_object;

            ~OperatorCallableT() override = default;

            explicit OperatorCallableT(T &t) : m_object(t) {}

            ReturnValue call(Args... args) override {
                return m_object(args...);
            }
        };

        template<typename T>
        struct ConstOperatorCallableT : public ICallable {
        public:
            const T &m_object;
            ReturnValue (C::* methodPtr)(Args...);

            ~ConstOperatorCallableT() override = default;

            explicit ConstOperatorCallableT(const T &t) : m_object(t) {}

            ReturnValue call(Args... args) override {
                return m_object(args...);
            }
        };

        ICallable *m_callable = nullptr;

    public:
        template<typename T, typename F>
        Method(T &t, F f) {
            m_callable = new CallableT<T, F, typename std::remove_reference<T>::type>(t, f);
        }

        template<typename T, typename F>
        Method(const T &t, F f) {
            m_callable = new ConstCallableT<T, F, typename std::remove_reference<T>::type>(t, f);
        }

        template<typename T>
        explicit Method(const T &t) {
            m_callable = new ConstOperatorCallableT<T>(t);
        }

        template<typename T>
        explicit Method(T &t) {
            m_callable = new OperatorCallableT<T>(t);
        }

        ~Method() {
            delete m_callable;
        }

        ReturnValue operator()(Args... args) {
            return (*m_callable).call(args...);
        }
    };
}