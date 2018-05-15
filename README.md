# node-gyp, nan, async function and TypeScript library project sample.

This is very simple but async native node-addon sample project.

## run

```sh
$ yarn global add node-gyp
$ git clone https://github.com/masataka-kurihara/gyp-nan-async-ts.git
$ cd gyp-nan-async-ts
$ yarn install
$ yarn test
```

## cpp source

```cpp
#include <nan.h>

using v8::Local;
using v8::String;
using Nan::Callback;
using Nan::New;

class HelloWorker : public Nan::AsyncWorker {
    public:
        HelloWorker(char* arg, Callback* callback): AsyncWorker(callback), name(arg) {}
        ~HelloWorker() {}
        void Execute () {}
    protected:
        void HandleOKCallback () {
            Local<v8::Value> argv[] = {
                // Nan::New converts char* to v8::MaybeLocal.
                // ToLocalChecked converts v8::MaybeLocal to v8::Local, then
                // 'name' will be deleted when the JS garbage collector run.
                String::Concat(New<String>("Hello ").ToLocalChecked(), New<String>(name).ToLocalChecked())
            };
            // async_resource comes from AsyncWorker's protected.
            callback->Call(1, argv, async_resource);
        }
    private:
        char* name;
};

NAN_METHOD(HelloMethod) {
    if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction())
        return Nan::ThrowTypeError("Illegal Arguments");

    Nan::Utf8String arg0(info[0]);

    // copy the strings since the JS garbage collector might run before the async request is finished
    // Nan::Utf8String's the overridden '*' operator return a char* value.
    char* str = new char[arg0.length() + 1];
    strcpy(str, *arg0);

    Callback *callback = new Callback(Nan::To<v8::Function>(info[1]).ToLocalChecked());
    Nan::AsyncQueueWorker(new HelloWorker(str, callback));
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target, New<String>("nativeHello").ToLocalChecked(),
        Nan::GetFunction(New<v8::FunctionTemplate>(HelloMethod)).ToLocalChecked());
}

NODE_MODULE(helloLib, Init)
```
