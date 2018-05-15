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
