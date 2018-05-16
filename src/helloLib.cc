#include <nan.h>

using namespace Nan;
using v8::Local;
using v8::String;
using v8::Value;
using v8::Function;

class HelloWorker : public AsyncWorker {
    public:
        HelloWorker(Callback* callback): AsyncWorker(callback) {}
        ~HelloWorker() {}
        void Execute () {/* Can't handle v8 objects here */}
    protected:
        void HandleOKCallback () {
            HandleScope scope;
            Local<String> hello = New<String>("Hello ").ToLocalChecked();
            Local<String> name = To<String>(GetFromPersistent("name")).ToLocalChecked();
            Local<Value> argv[] = { Null(), String::Concat(hello, name) };
            TryCatch try_catch;
            callback->Call(2, argv, async_resource);
            if (try_catch.HasCaught()) {
                FatalException(try_catch);
            }
        }
};

NAN_METHOD(HelloMethod) {
    if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction())
        return ThrowTypeError("Illegal Arguments");
    HandleScope scope;
    Callback *callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    HelloWorker *helloWorker = new HelloWorker(callback);
    helloWorker->SaveToPersistent("name", info[0]);
    AsyncQueueWorker(helloWorker);
}

NAN_MODULE_INIT(Init) {
    Export(target, "nativeHello", HelloMethod);
}

NODE_MODULE(helloLib, Init)
