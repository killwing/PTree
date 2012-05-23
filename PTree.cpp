#include "Exceptions.h"

template <typename Key>
PTree<Key>::Path::Path() {
}


template <typename Key>
PTree<Key>::Path::Path(const std::string& path) {
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

template <typename Key>
PTree<Key>::Path::Path(const char* s) {
    *this = Path(std::string(s));
}

template <typename Key> std::string
PTree<Key>::Path::pop() {
    if (p_.empty()) {
        return std::string();
    } else {
        std::string s = p_.front();
        p_.pop();
        return s;
    }
}

template <typename Key> void
PTree<Key>::Path::push(const std::string& s) {
    p_.push(s);
}

template <typename Key> bool
PTree<Key>::Path::empty() const {
    return p_.empty();
}

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
                throw PTreeBadPath("invalid path");
            }
        }

    } else {
        throw PTreeBadPath("path not found");
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
            const PTree* child = boost::any_cast<PTree>(&(it->second));
            if (child) {
                return child->get(path);
            } else {
                throw PTreeBadPath("invalid path");
            }
        }

    } else {
        throw PTreeBadPath("path not found");
    }
}

template <typename Key> 
template <typename T> T&
PTree<Key>::get(Path path) {
    std::string p = path.pop();
    typename TreeType::iterator it = data_.find(p);
    if (it != data_.end()) {
        if (path.empty()) {
            return ExConvertor<T>(it->second);
        } else {
            PTree* child = boost::any_cast<PTree>(&(it->second));
            if (child) {
                return child->get<T>(path);
            } else {
                throw PTreeBadPath("invalid path");
            }
        }

    } else {
        throw PTreeBadPath("path not found");
    }
}

template <typename Key> 
template <typename T> const T&
PTree<Key>::get(Path path) const {
    std::string p = path.pop();
    typename TreeType::const_iterator it = data_.find(p);
    if (it != data_.end()) {
        if (path.empty()) {
            return ExConstConvertor<T>(it->second);
        } else {
            const PTree* child = boost::any_cast<PTree>(&(it->second));
            if (child) {
                return child->get<T>(path);
            } else {
                throw PTreeBadPath("invalid path");
            }
        }

    } else {
        throw PTreeBadPath("path not found");
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
PTree<Key>::del(Path path) {
    using namespace boost;

    if (path.empty()) {
        return;
    }

    std::string p = path.pop();
    typename TreeType::iterator it = data_.find(p);
    if (path.empty()) {
        data_.erase(it);
    } else {
        PTree* child = boost::any_cast<PTree>(&(it->second));
        if (child) {
            child->del(path);
        }
    }
}

template <typename Key> void
PTree<Key>::keysTree(std::ostream& o, int indent) const {
    typename TreeType::const_iterator it = data_.begin();
    for (; it != data_.end(); ++it) {
        o << std::string(indent, ' ') << it->first << "\n";
        const PTree* child = boost::any_cast<PTree>(&(it->second));
        if (child) {
            child->keysTree(o, indent + 4);
        }
    }
}

