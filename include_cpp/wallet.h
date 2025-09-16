#pragma once

#include "my.h"
#include <glaze/glaze.hpp>

struct wallet
{
    int id;
    std::string name;
    std::string currency;
    std::string source;
    double      initial_amount;
    double      balance;
    std::string color;
    std::string created_at;
    std::string updated_at;
    bool is_active;
};

template <>
struct glz::meta<wallet> {
    static constexpr auto value = glz::object(
        "id",             &wallet::id,
        "name",           &wallet::name,
        "currency",       &wallet::currency,
        "source",         &wallet::source,
        "initial_amount", &wallet::initial_amount,
        "balance",        &wallet::balance,
        "color",          &wallet::color,
        "created_at",     &wallet::created_at,
        "updated_at",     &wallet::updated_at,
        "is_active",      &wallet::is_active
    );
};