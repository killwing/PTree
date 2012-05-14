#ifndef PTREE_H
#define PTREE_H

#include <map>
#include <queue>
#include <string>
#include <cassert>
#include <boost/any.hpp>

#include <iostream>

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
            throw "cast error";
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
            throw "cast error";
        }
    }
private:
    const boost::any& data_;
};

template <typename Key>
class PTree {
public:
    class Path {
    public:
        Path() {
        }

        Path(const std::string& path) {
            std::string::size_type start = 0;
            while (start < path.size()) {
                std::string::size_type pos = path.find(".", start);
                if (pos == std::string::npos) {
                    pos = path.size();                   
                } 
                p_.push(path.substr(start, pos - start));                
                start = pos + 1;                                         
            }
        }

        Path(const char* s) {
            *this = Path(std::string(s));
        }

        std::string pop() {
            if (p_.empty()) {
                return std::string();
            } else {
                std::string s = p_.front();
                p_.pop();
                return s;
            }
        }

        void push(const std::string& s) {
            p_.push(s);
        }
        
        bool empty() const {
            return p_.empty();
        }

    private:
        std::queue<Key> p_;
    };

    Convertor get(Path path);
    ConstConvertor get(Path path) const;
    void put(Path path, const boost::any& value);
    void push(Path path, const boost::any& value);
    void del(Path path);
    void keysTree(std::ostream& o, int indent = 0) const;

private:
    typedef std::map<Key, boost::any> TreeType;
    TreeType data_;
};


template <typename Key> Convertor
PTree<Key>::get(Path path) {
    std::string p = path.pop();
    typename TreeType::iterator it = data_.find(p);
    if (it != data_.end()) {
        if (path.empty()) {
            return Convertor(it->second);
        } else {
            PTree* child = boost::any_cast<PTree>(&(it->second));
            if (child) {
                return child->get(path);
            } else {
                throw "invalid path";
            }
        }
        
    } else {
        throw "path not found";
    }
}


template <typename Key> ConstConvertor
PTree<Key>::get(Path path) const {
    std::string p = path.pop();
    typename TreeType::const_iterator it = data_.find(p);
    if (it != data_.end()) {
        if (path.empty()) {
            return ConstConvertor(it->second);
        } else {
            PTree* child = boost::any_cast<PTree>(&(it->second));
            if (child) {
                return child->get(path);
            } else {
                throw "invalid path";
            }
        }
        
    } else {
        throw "path not found";
    }
}

template <typename Key> void 
PTree<Key>::put(Path path, const boost::any& value) {
    using namespace boost;

    if (path.empty()) {        
        return;
    }

    std::string p = path.pop();
    any& a = data_[p];
    if (path.empty()) {
        a = value;
    } else {        
        PTree* child = boost::any_cast<PTree>(&a);
        if (!child) {
            a = PTree(); // make a new tree
            child = boost::any_cast<PTree>(&a); // update
        }
        assert(child);
        child->put(path, value);
    }
}

template <typename Key> void
PTree<Key>::keysTree(std::ostream& o, int indent) const {
    typename TreeType::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        o << std::string(indent, ' ') << it->first << std::endl;
        const PTree* child = boost::any_cast<PTree>(&(it->second));
        if (child) {
            child->keysTree(o, indent + 4);
        }
    }
}

#endif

