#ifndef FEEDBACK_ITEM
#define FEEDBACK_ITEM

#include "testing/FeedbackSeverity.hpp"
#include "testing/SourceLocation.hpp"

class FeedbackItem
{
    public:
        FeedbackItem(SourceLocation start, std::string comment, FeedbackSeverity severity = FeedbackSeverity::NEUTRAL);
        FeedbackItem(SourceLocation start, SourceLocation end, std::string comment, FeedbackSeverity severity = FeedbackSeverity::NEUTRAL);

        SourceLocation start;
        SourceLocation end;

        std::string comment;
        FeedbackSeverity severity;

        bool HasEndLocation();

    private:
        bool _hasEnd;
};

#endif


