#pragma once

class LogTypes {
public:
    class Error {
    public:
        enum Value {
            UNDEFINED,
            UNKNOWN,
            INTERNAL,
            SYNTAX,
            TYPE_MISMATCH,
        };
    };

    class Warn {
    public:
        enum Value { SUGGESTION };
    };

    class Log {
    public:
        enum Value { INFO };
    };
};
