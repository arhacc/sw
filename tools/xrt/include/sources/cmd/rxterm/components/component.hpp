#pragma once

#include <string>
#include <memory>
#include <sources/cmd/rxterm/image.hpp>
#include <sources/cmd/rxterm/components/text.hpp>

namespace rxterm {

    struct Renderable {
        virtual Image render(unsigned w) const = 0;

        virtual ~Renderable() = default;
    };

    auto isImage(Image const &x) { return x; }

    template<class R> auto getImage(unsigned const w, R const &r) -> decltype(isImage(r.render(w))) {
        return r.render(w);
    }

    template<class R> auto getImage(unsigned const w, R const &r) -> decltype(isImage(r(w))) {
        return r(w);
    }

    template<class R> auto getImage(unsigned const w, R const &r) -> decltype(isImage(r)) {
        return w;
    }

    template<class R> auto getImage(unsigned const w, R const &r) -> decltype(Text{{}, toString(r)}.render(w)) {
        return Text{{}, toString(r)}.render(w);
    }


    struct Component : Renderable {

        template<class T> Component(T const &model)
                : ptr{std::make_shared<Model < T>>(model)} {}

        Image render(unsigned const w) const override {
            return ptr->render(w);
        }


        template<class T> struct Model : Renderable {
            T const model;

            Model(T const &data) : model{data} {}

            Image render(unsigned const w) const override {
                return getImage(w, model);
            }
        };

        std::shared_ptr<Renderable> ptr;

    };

}
