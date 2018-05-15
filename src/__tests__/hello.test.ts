import { greeting } from '../hello';

test('greeting', (done) => {
    greeting('World!', (msg) => {
        console.warn(msg);
        done();
    });
});
