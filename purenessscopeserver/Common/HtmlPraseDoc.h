// -*- mode: c++; tab-width: 2; indent-tabs-mode: nil; -*-
//----------------------------------------------------------------------------
// A simple object oriented HTML document builder for C++.
//
// Created by Marcus Geelnard, 2014-09-28
//----------------------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
//----------------------------------------------------------------------------

/// @mainpage
/// A simple object oriented HTML document builder for C++.
///
/// @section example Example usage
/// @code{.cpp}
///   htmlgen::Document doc;
///
///   // Generate a document structure.
///   htmlgen::Document::Element* body = doc.root()->AddChild("body");
///   htmlgen::Document::Element* a = body->AddChild("a");
///   a->AddAttribute("href", "http://unlicense.org/");
///   a->AddTextChild("Click on me!");
///   htmlgen::Document::Element* p = body->AddChild("p");
///   p->AddTextChild("Hello world!");
///
///   // Convert the document to an HTML formatted string.
///   std::string html_string;
///   doc.GetHTML(html_string);
///   std::cout << html_string;
/// @endcode

#ifndef _HTML_DOCUMENT_H_
#define _HTML_DOCUMENT_H_

#include <cstring>
#include <string>
#include <vector>
using namespace std;

/// @brief A container for a single HTML document.
///
/// The Document contains a root node, which is an Element with the name
/// "html". Children can be added to the root node to form a node tree.
class HtmlDocument
{
public:
    /// @brief An interface used for all HTML nodes.
    class Node
    {
    public:
        virtual ~Node() {}

        /// @brief Get an HTML formatted string representing this node.
        /// @param[out] out The output string that will receive the HTML.
        virtual void GetHTML(string& out) const = 0;
    };

    /// @brief An attribute that can be part of an Element.
    class Attribute
    {
    public:
        Attribute(const char* name, const char* value) : name_(name)
        {
            SetEscapedValue(value, strlen(value));
        }

        Attribute(const string& name, const string& value) :
            name_(name)
        {
            SetEscapedValue(value.data(), value.size());
        }

        void GetHTML(string& out) const
        {
            out.append(name_);
            out.append("=\"", 2);
            out.append(value_);
            out += '"';
        }

    private:
        void SetEscapedValue(const char* value, size_t len)
        {
            // We escape: " (0x22), & (0x26) and < (0x3c). The escape LUT is
            // indexed by the character code (0-255) modulo 8.
            static const char kEscapeLut[8] = { 1, 0, '"', 0, '<', 0, '&', 0 };

            // Reserve space for the escaped string.
            // Note: This is optimized for strings that need no escaping. To
            // optimize for strings that may need escaping, but at some memory
            // cost, reserve len + (len >> 1) instead.
            value_.reserve(len);

            // Copy the value string, and escape characters as needed.
            for (size_t i = 0; i < len; ++i)
            {
                char c = value[i];

                if (kEscapeLut[c & 7] == c)
                {
                    switch (c)
                    {
                    case '"':
                        value_.append("&#34;", 5);
                        break;

                    case '&':
                        value_.append("&amp;", 5);
                        break;

                    default:  // '<'
                        value_.append("&lt;", 4);
                    }
                }
                else
                {
                    value_ += c;
                }
            }
        }

        string name_;
        string value_;
    };

    /// @brief A text node (typically named "#text" in a DOM).
    class TextNode : public Node
    {
    public:
        explicit TextNode(const char* value)
        {
            SetEscapedValue(value, strlen(value));
        }

        explicit TextNode(const string& value)
        {
            SetEscapedValue(value.data(), value.size());
        }

        virtual void GetHTML(string& out) const
        {
            out.append(value_);
        }

    private:
        void SetEscapedValue(const char* value, size_t len)
        {
            // We escape: & (0x26), < (0x3c) and > (0x3e). The escape LUT is
            // indexed by the character code (0-255) modulo 16.
            static const char kEscapeLut[16] = { 1, 0, 0, 0, 0, 0, '&', 0,
                                                 0, 0, 0, 0, '<', 0, '>', 0
                                               };

            // Reserve space for the escaped string.
            // Note: This is optimized for strings that need no escaping. To
            // optimize for strings that may need escaping, but at some memory
            // cost, reserve len + (len >> 1) instead.
            value_.reserve(len);

            // Copy the value string, and escape characters as needed.
            for (size_t i = 0; i < len; ++i)
            {
                char c = value[i];

                if (kEscapeLut[c & 15] == c)
                {
                    switch (c)
                    {
                    case '&':
                        value_.append("&amp;", 5);
                        break;

                    case '<':
                        value_.append("&lt;", 4);
                        break;

                    default:  // '>'
                        value_.append("&gt;", 4);
                    }
                }
                else
                {
                    value_ += c;
                }
            }
        }

        string value_;
    };

    /// @brief An Element can have attributes and children.
    class Element : public Node
    {
    public:
        explicit Element(const char* name) : name_(name) {}

        explicit Element(const string& name) : name_(name) {}

        virtual ~Element()
        {
            int n4Size = (int)children_.size();

            for (int iLoop = 0; iLoop < n4Size; iLoop ++)
            {
                delete (children_[iLoop]);
            }
        }

        virtual void GetHTML(string& out) const
        {
            out += '<';
            out.append(name_);

            int n4Size = (int)attributes_.size();

            for (int iLoop = 0; iLoop < n4Size; iLoop ++)
            {
                out += ' ';
                attributes_[iLoop].GetHTML(out);
            }

            if (children_.size() > 0 || !IsVoidElement())
            {
                out += '>';

                n4Size = (int)children_.size();

                for (int iLoop = 0; iLoop < n4Size; iLoop ++)
                {
                    (children_[iLoop])->GetHTML(out);
                }

                out.append("</", 2);
                out.append(name_);
            }

            out += '>';
        }

        /// @brief Add an attribute to this Element.
        /// @param name The attribute name.
        /// @param value The attribute value (unescaped).
        void AddAttribute(const char* name, const char* value)
        {
            attributes_.push_back(Attribute(name, value));
        }

        /// @brief Add an attribute to this Element.
        /// @param name The attribute name.
        /// @param value The attribute value (unescaped).
        void AddAttribute(const string& name, const string& value)
        {
            attributes_.push_back(Attribute(name, value));
        }

        /// @brief Add a child to this Element.
        /// @param name The name of the new child element.
        /// @returns The newly created Element.
        Element* AddChild(const char* name)
        {
            children_.push_back(new Element(name));
            return reinterpret_cast<Element*>(children_.back());
        }

        /// @brief Add a child to this Element.
        /// @param name The name of the new child element.
        /// @returns The newly created Element.
        Element* AddChild(const string& name)
        {
            children_.push_back(new Element(name));
            return reinterpret_cast<Element*>(children_.back());
        }

        /// @brief Add a text node child to this element.
        /// @param value The text for the new text node (unescaped).
        void AddTextChild(const char* value)
        {
            children_.push_back(new TextNode(value));
        }

        /// @brief Add a text node child to this element.
        /// @param value The text for the new text node (unescaped).
        void AddTextChild(const string& value)
        {
            children_.push_back(new TextNode(value));
        }

    private:
        /// @brief Determine if this is a void element.
        ///
        /// The <a href="http://www.w3.org/TR/html5/syntax.html#void-elements">
        /// HTML5 specification</a> defines a set of "void elements" that must
        /// not have end tags.
        /// @returns true if this is a void element.
        /// @note The method is case sensitive, so an element with the name
        /// "img" is treated as a void element, while an element with the name
        /// "IMG" is not.
        bool IsVoidElement() const
        {
            static const char* const kVoidNames[] =
            {
                "area", "base", "br", "col", "embed", "hr", "img", "input",
                "keygen", "link", "meta", "param", "source", "track", "wbr"
            };
            static const int kNumVoidNames =
                sizeof(kVoidNames) / sizeof(kVoidNames[0]);

            // Do a binary search.
            int imin = 0, imax = kNumVoidNames - 1;

            while (imax >= imin)
            {
                int imid = (imin + imax) / 2;
                int diff = strcmp(kVoidNames[imid], name_.data());

                if (diff == 0)
                {
                    return true;
                }
                else if (diff < 0)
                {
                    imin = imid + 1;
                }
                else
                {
                    imax = imid - 1;
                }
            }

            return false;
        }

        const string name_;
        vector<Attribute> attributes_;
        vector<Node*> children_;
    };

    HtmlDocument() : root_(Element("html")) {}

    /// @brief Get the root element of this document.
    Element* root()
    {
        return &root_;
    }

    /// @brief Get an HTML formatted string representing this document.
    /// @param[out] out The output string that will receive the HTML.
    void GetHTML(string& out) const
    {
        out.append("<!DOCTYPE html>\n");
        root_.GetHTML(out);
        out += '\n';
    }

private:
    Element root_;
};

#endif // DOCUMENT_H_