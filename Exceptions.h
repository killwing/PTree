#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <stdexcept>

class PTreeError : public std::runtime_error {
public:
    PTreeError(const std::string& what)
        : std::runtime_error(what) {
    }

protected:
    ~PTreeError() throw() {
    }
};

class PTreeBadPath : public PTreeError {
public:
    PTreeBadPath(const std::string& what)
        : PTreeError(what) {
    }
};

class PTreeBadType : public PTreeError {
public:
    PTreeBadType(const std::string& what)
        : PTreeError(what) {
    }
};

#endif // EXCEPTIONS_H

