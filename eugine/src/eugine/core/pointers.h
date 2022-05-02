//
// Created by aiden on 5/1/22.
//

#ifndef EUGINE_POINTERS_H
#define EUGINE_POINTERS_H

namespace eg {

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

#endif //EUGINE_POINTERS_H
