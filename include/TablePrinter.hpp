#ifndef TABLEPRINTER_H
#define TABLEPRINTER_H
#include <iostream>
#include <vector>
#include <string>
#include <fmt/core.h>

// namespace DavidM-Fox
namespace dmf {
// namespace tableprinter
namespace tableprinter {

enum class Align { LEFT, RIGHT, CENTER };
enum class Separator { LEFT, RIGHT, BOTH, NONE };

class Component {
public:
    Component() {}

    virtual void format() = 0;

    class Format {
    public:
        Format(const Separator &sep = Separator::NONE,
               const Align &alignment = Align::CENTER, const size_t &width = 0,
               const std::string &additional = "")
            : separator(sep), alignment(alignment), width(width),
              additional(additional)
        {
        }

        const std::string &operator()() { return fmt_string; }

        void format()
        {
            std::string fmt = "{:" +
                              fmt::format("{}{}{}", align_char(alignment),
                                          width, additional) +
                              "}";
            fmt_string = fmt::format(sep_string(separator), fmt);
        }

        Separator separator;
        Align alignment;
        size_t width;
        std::string additional;
        std::string fmt_string;
    };

protected:
    static const std::string align_char(const Align &index)
    {
        static const std::string a[] = {"<", ">", "^"};
        return a[static_cast<int>(index)];
    }

    static const std::string sep_string(const Separator &index)
    {
        static const std::string a[] = {"|{}", "{}|", "|{}|", "{}"};
        return a[static_cast<int>(index)];
    }
};

class Column : Component {
public:
    Column(const std::string &text)
        : header_text(text),
          header_fmt({Separator::LEFT, Align::CENTER, text.size() + 2, ""}),
          data_fmt({Separator::LEFT, Align::CENTER, text.size() + 2, ""}),
          column_width(header_text.size() + 2)
    {
    }
    Column(const std::string &header_text, const Component::Format &header_fmt,
           const Component::Format &data_fmt)
        : header_text(header_text), header_fmt(header_fmt), data_fmt(data_fmt),
          column_width(header_text.size() + 2)
    {
    }

    void format() override
    {
        formatHeader();
        formatData();
    }
    void formatHeader() { header_fmt.format(); }
    void formatData() { data_fmt.format(); }

    void setWidth(const size_t &width = 0)
    {
        if (width == 0 || width < header_text.size() + 2)
            column_width = header_text.size() + 2;
        else
            column_width = width;
        header_fmt.width = column_width;
        data_fmt.width = column_width;
    }
    const size_t &getWidth() { return column_width; }

    std::string header_text;
    Component::Format header_fmt;
    Component::Format data_fmt;

private:
    size_t column_width;
};

class Title : public Component {
public:
    Title(const std::string &text)
        : text(text), fmt({Separator::BOTH, Align::CENTER, text.size() + 2, ""})
    {
    }
    Title(const std::string &text, const Component::Format &title_fmt)
        : text(text), fmt(title_fmt)
    {
    }

    void format() override { fmt.format(); }

    void setFmtWidth(const size_t &width = 0)
    {
        if (width == 0 || width < text.size() + 2)
            fmt.width = text.size() + 2;
        else
            fmt.width = width;
    }

    std::string text;
    Component::Format fmt;
};

typedef std::vector<std::pair<std::string, double>> form;

class FormPrinter {
public:
    FormPrinter(const std::string &title, const form &data)
        : title(title), data(data), field_fmt({}), value_fmt({})
    {
    }

    void autoFormat()
    {
        // Get max width of form field/value
        size_t max_field_w = 0;
        size_t max_value_w = 0;
        for (auto &pair : data) {
            size_t field_w = (pair.first.size() + 2);
            size_t value_w = fmt::format("{:.0f}", pair.second).size() + 2;
            max_field_w = (field_w > max_field_w) ? field_w : max_field_w;
            max_value_w = (value_w > max_value_w) ? value_w : max_value_w;
        }
        field_fmt = {Separator::LEFT, Align::LEFT, max_field_w, ""};
        field_fmt.format();

        value_fmt = {Separator::RIGHT, Align::RIGHT, max_value_w, ""};
        value_fmt.format();

        title.setFmtWidth(max_field_w + max_value_w);
        title.format();
    }

    void print()
    {
        printHeading();
        printFormData();
    }

    void printHeading()
    {
        title_str = fmt::format(title.fmt(), title.text);
        size_t title_w = title_str.size();
        std::cout << std::string(title_w, '-') << "\n" << title_str << "\n";
        std::cout << std::string(title_w, '-') << "\n";
    }

    void printFormData()
    {
        for (auto &pair : data) {
            fmt::print(field_fmt(), pair.first);
            fmt::print(value_fmt(), pair.second);
            std::cout << "\n";
        }
    }

    Component::Format field_fmt;
    Component::Format value_fmt;

    Title title;
    form data;

private:
    std::string title_str;
};

class TablePrinter {
public:
    TablePrinter(const std::string &title) : title(title) {}

    TablePrinter(const std::string &title,
                 const std::vector<std::string> &headers)
        : title(title)
    {
        for (auto &text : headers) {
            columns.push_back({text});
        }
    }

    void formatHeading()
    {
        headers_str = "";
        for (auto &col : columns) {
            col.format();
            headers_str += fmt::format(col.header_fmt(), col.header_text);
        }
        headers_str += "|";

        title.setFmtWidth(std::max(title.fmt.width, headers_str.size() - 2));
        title.format();
        title_str = fmt::format(title.fmt(), title.text);
    }

    void printHeading()
    {
        size_t title_w = title_str.size();
        size_t headers_w = headers_str.size();
        std::cout << std::string(title_w, '-') << "\n" << title_str << "\n";
        std::cout << std::string(std::max(title_w, headers_w), '-') << "\n";
        std::cout << headers_str << "\n" << std::string(headers_w, '-') << "\n";
    }

    void printDataRow(const std::vector<double> &data)
    {
        if (data.size() != columns.size()) {
            std::cerr << "error here......\n";
            return;
        }

        for (size_t i = 0; i < data.size(); ++i) {
            fmt::print(columns[i].data_fmt(), data[i]);
        }
        std::cout << "|\n";
    }

    void printFormRow(std::pair<std::string, double> data) {}

    Title title;
    std::vector<Column> columns;

private:
    std::string title_str;
    std::string headers_str;
};

} // namespace tableprinter
} // namespace dmf
#endif // !TABLEPRINTER_H
