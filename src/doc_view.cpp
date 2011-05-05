#include <signal.hpp>

#include <iostream>
#include <string>
#include <functional>

class Document
{
public:
    typedef feng::signal<void,bool>  signal_t;
    typedef signal_t::connection_type  connection_t;

public:
    Document()
    {}

    template<typename F>
    connection_t connect( const F& subscriber)
    {
        return m_sig.connect(subscriber);
    }

    void disconnect(connection_t subscriber)
    {
        m_sig.disconnect(subscriber);
    }

    void append(const char* s)
    {
        m_text += s;
        m_sig(true);
    }

    const std::string& getText() const
    {
        return m_text;
    }

private:
    signal_t    m_sig;
    std::string m_text;
};

class View
{
public:
    View(Document& m)
        : m_document(m)
    {
        m_connection = m_document.connect(std::bind(&View::refresh, this, std::placeholders::_1));
    }

    virtual ~View()
    {
        m_document.disconnect(m_connection);
    }

    virtual void refresh(bool bExtended) const = 0;

protected:
    Document&               m_document;

private:
    Document::connection_t  m_connection;
};

class TextView : public View
{
public:
    TextView(Document& doc)
        : View(doc)
    {}

    virtual void refresh(bool bExtended) const
    {
        std::cout << "TextView: " << m_document.getText() << std::endl;
    }
};

class HexView : public View
{
public:
    HexView(Document& doc)
        : View(doc)
    {}

    virtual void refresh(bool bExtended) const
    {
        const std::string&  s = m_document.getText();

        std::cout << "HexView:";

        for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
            std::cout << ' ' << std::hex << static_cast<int>(*it);

        std::cout << std::endl;
    }
};

int main(int argc, char* argv[])
{
    Document    doc;
    TextView    v1(doc);
    HexView     v2(doc);


    //std::cerr << "\naddress of doc is " << (std::ptrdiff_t)(&doc) << "\n";
    //std::cerr << "\naddress of v1 is " << (std::ptrdiff_t)(&v1) << "\n";
    //std::cerr << "\naddress of v2 is " << (std::ptrdiff_t)(&v2) << "\n";

    doc.append(argc == 2 ? argv[1] : "Hello world!");
    return 0;
}
