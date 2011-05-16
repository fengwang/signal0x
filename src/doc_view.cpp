#include <signal.hpp>
#include <iostream>
#include <string>
#include <functional>
struct Document
{
    typedef signal0x::signal<void,bool>  signal_t;
    typedef signal_t::connection_type  connection_t;

    template<typename F>
    connection_t connect( const F& subscriber)
    { return m_sig.connect(subscriber); }

    void disconnect(connection_t subscriber)
    { m_sig.disconnect(subscriber); }

    void append(const char* s)
    { m_text += s; m_sig(true); }

    const std::string& getText() const
    { return m_text; }
private:
    signal_t    m_sig;
    std::string m_text;
};
struct View
{
    View(Document& m) : m_document(m) { m_connection = m_document.connect(std::bind(&View::refresh, this, std::placeholders::_1)); }
    virtual ~View() { m_document.disconnect(m_connection); }
    virtual void refresh(bool bExtended) const = 0;
protected:
    Document&               m_document;
private:
    Document::connection_t  m_connection;
};
struct TextView : public View
{
    TextView(Document& doc) : View(doc) {}
    virtual void refresh(bool bExtended) const
    { std::cout << "TextView: " << m_document.getText() << std::endl; }
};
struct HexView : public View
{
    HexView(Document& doc) : View(doc) {}
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
    doc.append(argc == 2 ? argv[1] : "Hello world!");
    return 0;
}
