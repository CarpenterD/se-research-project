#include "testing/FeedbackItem.hpp"

#include <stdexcept>

FeedbackItem::FeedbackItem(SourceLocation start, std::string comment, FeedbackSeverity severity) :
start(start), end(end), comment(comment), severity(severity)
{
    _hasEnd = false;
};

FeedbackItem::FeedbackItem(SourceLocation start, SourceLocation end, std::string comment, FeedbackSeverity severity) :
start(start), end(end), comment(comment), severity(severity)
{
    _hasEnd = true;

    if (start.file != end.file) {
        throw std::invalid_argument("Invalid feedback location. Endpoints in different files.");
    }
};

bool FeedbackItem::HasEndLocation(){
    return _hasEnd;
}
