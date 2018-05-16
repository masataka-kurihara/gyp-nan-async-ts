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

using namespace Nan;
using namespace v8;

class HelloWorker : public AsyncWorker {
    public:
        HelloWorker(char* arg, Callback* callback): AsyncWorker(callback), name(arg) {}
        ~HelloWorker() {}
        void Execute () {}
    protected:
        void HandleOKCallback () {
            Nan::HandleScope scope;
            Local<Value> argv[] = {
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
        return ThrowTypeError("Illegal Arguments");

    Nan::HandleScope scope;
    Utf8String arg0(info[0]);

    // copy the strings since the JS garbage collector might run before the async request is finished
    // Nan::Utf8String's the overridden '*' operator return a char* value.
    char* str = new char[arg0.length() + 1];
    strcpy(str, *arg0);

    Callback *callback = new Callback(To<Function>(info[1]).ToLocalChecked());
    AsyncQueueWorker(new HelloWorker(str, callback));
}

NAN_MODULE_INIT(Init) {
    Export(target, "nativeHello", HelloMethod);
}

NODE_MODULE(helloLib, Init)
```

## TypeScript

```typescript
const binding = require('bindings')('helloLib');

export interface Cb {
    (msg: string): void;
}

export const greeting = (name: string, cb: Cb): void => {
    return binding.nativeHello(name, cb);
};

export const greetingPromise = (name: string): Promise<string> => {
    return new Promise<string>((resolve) => {
        binding.nativeHello(name, (msg: string) => {
            resolve(msg);
        });
    });
};
```
