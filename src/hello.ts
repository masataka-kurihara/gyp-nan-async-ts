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
