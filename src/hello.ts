const binding = require('bindings')('helloLib');

export interface Cb {
    (msg: string): void;
}

export const greeting = (name: string, cb: Cb): void => {
    return binding.nativeHello(name, cb);
};
