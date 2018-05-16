import { greeting, greetingPromise } from '../hello';

describe('hello', () => {
    test('greeting', (done) => {
        greeting('World!', (err, msg) => {
            expect(err).toBeNull();
            expect(msg).toBe('Hello World!');
            done();
        });
    });

    test('greetingPromise', async () => {
        expect(await greetingPromise('Promise!')).toBe('Hello Promise!');
    });
});
