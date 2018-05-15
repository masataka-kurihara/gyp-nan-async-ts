import { greeting, greetingPromise } from '../hello';

describe('hello', () => {
    test('greeting', (done) => {
        greeting('World!', (msg) => {
            console.warn(msg);
            done();
        });
    });

    test('greetingPromise', async () => {
        console.warn(await greetingPromise('Promise World!'));
    });
});
