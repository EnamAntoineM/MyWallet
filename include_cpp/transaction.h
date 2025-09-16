#pragma once

#include "my.h"
#include <glaze/glaze.hpp>

struct transaction
{
    int id;
    std::string wallet_name;
    std::string type;
    std::string recorded_at;
    std::string updated_at;
    std::string category;
    std::string description;
    double      amount;
    double is_archived;
    std::string related_wallet_name;
};

template <>
struct glz::meta<transaction> {
    static constexpr auto value = glz::object(
        "id",                  &transaction::id,
        "wallet_name",         &transaction::wallet_name,
        "type",                &transaction::type,
        "recorded_at",         &transaction::recorded_at,
        "updated_at",          &transaction::updated_at,
        "category",            &transaction::category,
        "description",         &transaction::description,
        "amount",              &transaction::amount,
        "is_archived",         &transaction::is_archived,
        "related_wallet_name", &transaction::related_wallet_name
    );
};