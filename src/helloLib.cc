#include <nan.h>

using namespace Nan;
using namespace v8;

class HelloWorker : public AsyncWorker {
    public:
        HelloWorker(Callback* callback): AsyncWorker(callback) {}
        ~HelloWorker() {}
        void Execute () {}
    protected:
        void HandleOKCallback () {
            Nan::HandleScope scope;//
            Local<String> hello = New<String>("Hello ").ToLocalChecked();
            Local<String> name = To<String>(GetFromPersistent("name")).ToLocalChecked();
            Local<Value> argv[] = { Null(), String::Concat(hello, name) };
            // async_resource is AsyncWorker's protected member.
            callback->Call(2, argv, async_resource);
        }
};

NAN_METHOD(HelloMethod) {
    if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction())
        return ThrowTypeError("Illegal Arguments");

    Nan::HandleScope scope;
    Callback *callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    HelloWorker *helloWorker = new HelloWorker(callback);
    helloWorker->SaveToPersistent("name", info[0]);
    AsyncQueueWorker(helloWorker);
}

NAN_MODULE_INIT(Init) {
    Export(target, "nativeHello", HelloMethod);
}

NODE_MODULE(helloLib, Init)
