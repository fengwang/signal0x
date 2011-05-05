#include <signal.hpp>
#include <functional>
#include <string>
#include <iostream>
 
using namespace std;
 
// 被观察目标
struct CSubject{
   
    typedef sig::signal<void, string> sig_t;
    sig_t m_sig;
 
    // 发布信息
    void publish(string news)
    {
        m_sig(news);
    }
    // 订阅
    template<typename S>
    void subscribe(const S &observer)
    {
        m_sig.connect(observer);
    }
};
 
// 观察者
struct CObserver{
    // 有新消息时被回调
    void newsIncoming(string s)
    {
        cout << this << " received:" << s << endl;
    }   
    // 声明观察目标
    void watch(CSubject& subject)
    {
        subject.subscribe(std::bind(&CObserver::newsIncoming, this, std::placeholders::_1));
    }
};
 
int main(int argc, char* argv[])
{
    CSubject a,b,c;
 
    CObserver x,y,z;
 
    x.watch(a);
    y.watch(a);
    z.watch(a);
    a.publish("hello, I'm a");
    b.publish("hello, I'm b");
    c.publish("hello, I'm c");
 
    cout << "\n";

    y.watch(b);
    z.watch(c);
    a.publish("hello, I'm a");
    b.publish("hello, I'm b");
    c.publish("hello, I'm c");
 
    return 0;
}
