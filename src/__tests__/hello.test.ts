import { greeting, greetingPromise } from '../hello';

describe('hello', () => {
    test('greeting', (done) => {
        greeting('World!', (err, msg) => {
            if (err) {
                fail(err);
            }
            console.warn(msg);
            done();
        });
    });

    test('greetingPromise', async () => {
        try {
            console.warn(await greetingPromise('Promise World!'));
        } catch (err) {
            fail(err);
        }
    });
});
