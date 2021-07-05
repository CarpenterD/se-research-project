#include "ConfigReader.hpp"

#include <string>
#include "ConfigConverter.hpp"

void ConfigReader::internal::ParseStdTestOptions(TestConfig &config)
{
    {%- for opt in options %}
    {%- if opt.optional %}
    config.{{opt.GetPropertyName()}} = ConfigConverter::ConvertOption<{{opt.GetPropertyType()}}>(ConfigReader::GetConfigOption(config.Options, "{{opt.GetPropertyName()}}", "StdOptions"), config.{{opt.GetDefaultPropertyName()}});
    {%- else %}
    config.{{opt.GetPropertyName()}} = ConfigConverter::ConvertOption<{{opt.GetPropertyType()}}>(ConfigReader::GetConfigOption(config.Options, "{{opt.GetPropertyName()}}", "StdOptions"));
    {%- endif %}
{%- endfor %}
}

