#ifndef CONVERTORS_H
#define CONVERTORS_H

#include "Exceptions.h"

#include <boost/any.hpp>

class Convertor {
public:
    Convertor(boost::any& a) : data_(a) {
    }

    template <typename T>
    operator T&() {
        T* v = boost::any_cast<T>(&data_);
        if (v) {
            return *v;
        } else {
            throw PTreeBadType("cast error");
        }
    }
private:
    boost::any& data_;
};

class ConstConvertor {
public:
    ConstConvertor(const boost::any& a) : data_(a) {
    }

    template <typename T>
    operator T&() const {
        const T* cv = boost::any_cast<T>(&data_);
        // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#976
        T* v = const_cast<T*>(cv);
        if (v) {
            return *v;
        } else {
            throw PTreeBadType("const cast error");
        }
    }
private:
    const boost::any& data_;
};

#endif // CONVERTORS_H

