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
```

## ts source

```typescript
const binding = require('bindings')('helloLib');

export interface Cb {
    (err: Error, msg: string): void;
}

export const greeting = (name: string, cb: Cb): void => {
    return binding.nativeHello(name, cb);
};

export const greetingPromise = (name: string): Promise<string> => {
    return new Promise<string>((resolve, reject) => {
        binding.nativeHello(name, (err: Error, msg: string) => {
            if (err) {
                reject(err);
            } else {
                resolve(msg);
            }
        });
    });
};
```
