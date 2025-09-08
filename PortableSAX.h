#pragma once
#include <string>
#include <vector>
#include <utility>

struct PortableAttribute {
    std::string localName;
    std::string value;
};

struct PortableAttributes {
    std::vector<PortableAttribute> items;
    int getLength() const { return static_cast<int>(items.size()); }
};
