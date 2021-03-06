#include "sqlitebegintrans.h"
#include "sqlitequerytype.h"

#include <parser/statementtokenbuilder.h>

SqliteBeginTrans::SqliteBeginTrans()
{
    queryType = SqliteQueryType::BeginTrans;
}

SqliteBeginTrans::SqliteBeginTrans(const SqliteBeginTrans& other) :
    SqliteQuery(other), name(other.name), transactionKw(other.transactionKw), type(other.type)
{
}

SqliteBeginTrans::SqliteBeginTrans(SqliteBeginTrans::Type type, bool transactionKw, const QString& name)
    : SqliteBeginTrans()
{
    this->type = type;
    this->transactionKw = transactionKw;
    this->name = name;
}

SqliteBeginTrans::SqliteBeginTrans(bool transactionKw, const QString &name)
{
    this->transactionKw = transactionKw;
    this->name = name;
}

SqliteStatement*SqliteBeginTrans::clone()
{
    return new SqliteBeginTrans(*this);
}

QString SqliteBeginTrans::typeToString(SqliteBeginTrans::Type type)
{
    switch (type)
    {
        case Type::null:
            return QString();
        case Type::DEFERRED:
            return "DEFERRED";
        case Type::IMMEDIATE:
            return "IMMEDIATE";
        case Type::EXCLUSIVE:
            return "EXCLUSIVE";
    }
    return QString();
}

TokenList SqliteBeginTrans::rebuildTokensFromContents()
{
    StatementTokenBuilder builder;
    builder.withTokens(SqliteQuery::rebuildTokensFromContents());
    builder.withKeyword("BEGIN");

    if (type != Type::null)
        builder.withSpace().withKeyword(typeToString(type));

    if (transactionKw)
    {
        builder.withSpace().withKeyword("TRANSACTION");
        if (!name.isNull())
            builder.withSpace().withOther(name);
    }

    builder.withOperator(";");

    return builder.build();
}
