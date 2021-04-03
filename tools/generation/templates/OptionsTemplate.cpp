#include "{{headerName}}"

#include <iostream>
#include "utils/ConfigReader.hpp"
#include "utils/ConfigConverter.hpp"

void {{set.GetClassName()}}::Initialise(std::vector<ConfigOption> &options)
{
#ifdef DEBUG
    std::clog << "Initialising {{set.GetClassName()}}" << std::endl;
#endif
{%- for opt in set.options %}
    {%- if opt.isList %}
        {%- if opt.optional %}
    ConfigConverter::ConvertOption<{{opt.GetBasePropertyType()}}>(ConfigReader::GetConfigOption(options, "{{opt.GetPropertyName()}}", "{{set.GetClassName()}}"), {{opt.GetPropertyName()}}, {{opt.GetDefaultPropertyName()}});
        {%- else %}
    ConfigConverter::ConvertOption<{{opt.GetBasePropertyType()}}>(ConfigReader::GetConfigOption(options, "{{opt.GetPropertyName()}}", "{{set.GetClassName()}}"), {{opt.GetPropertyName()}});
        {%- endif %}
    {%- else %}
        {%- if opt.optional %}
    {{opt.GetPropertyName()}} = ConfigConverter::ConvertOption<{{opt.GetPropertyType()}}>(ConfigReader::GetConfigOption(options, "{{opt.GetPropertyName()}}", "{{set.GetClassName()}}"), {{opt.GetDefaultPropertyName()}});
        {%- else %}
    {{opt.GetPropertyName()}} = ConfigConverter::ConvertOption<{{opt.GetPropertyType()}}>(ConfigReader::GetConfigOption(options, "{{opt.GetPropertyName()}}", "{{set.GetClassName()}}"));
        {%- endif %}
    {%- endif %}
{%- endfor %}
}
