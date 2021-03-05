#ifndef {{set.GetDefineName()}}
#define {{set.GetDefineName()}}

#include <string>

{{set.GetClassDocumentation()}}
struct {{set.GetClassName()}}
{
{%- for opt in set.options %}
        {{opt.GetPropertyType()}} {{opt.GetPropertyName()}}; {{ "//!< "+opt.GetDocString() if opt.description }}
    {%- if opt.optional %}
        const {{opt.GetPropertyType()}} {{opt.GetDefaultPropertyName()}} = {{opt.GetDefaultLiteral()}}; //!< default value for '{{opt.GetPropertyName()}}' if no value is specified
    {%- endif %}
{%- endfor %}
};

#endif