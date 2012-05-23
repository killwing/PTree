#ifndef PTREE_H
#define PTREE_H

#include "Convertors.h"

#include <map>
#include <queue>
#include <string>
#include <cassert>
#include <boost/any.hpp>

template <typename Key>
class PTree {
public:
    class Path {
    public:
        Path();
        Path(const std::string& path);
        Path(const char* s);

        std::string pop();
        void push(const std::string& s);
        bool empty() const;

    private:
        std::queue<Key> p_;
    };

    Convertor get(Path path);
    ConstConvertor get(Path path) const;

    template <typename T>
    T& get(Path path);

    template <typename T>
    const T& get(Path path) const;

    void put(Path path, const boost::any& value);
    void push(Path path, const boost::any& value);
    void del(Path path);

    void keysTree(std::ostream& o, int indent = 0) const;

private:
    typedef std::map<Key, boost::any> TreeType;
    TreeType data_;
};

// include implementation
#include "PTree.cpp"

#endif // PTREE_H

