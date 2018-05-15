const lib = require('bindings')('helloLib');
export { lib };

export interface Cb {
    (msg: string): void;
}

export const greeting = (name: string, cb: Cb): void => {
    return lib.nativeHello(name, cb);
};
