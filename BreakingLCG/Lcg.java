import java.util.Locale;

public final class Lcg {
    // Java's Random LCG parameters
    private static final long MULTIPLIER = 0x5DEECE66DL;
    private static final long ADDEND     = 0xBL;
    private static final long MASK       = (1L << 48) - 1;

    // Internal 48-bit state
    private long seed;

    public Lcg(long seed) {
        setSeed(seed);
    }

    /** Matches java.util.Random seeding: (seed ^ MULTIPLIER) & MASK */
    public void setSeed(long s) {
        this.seed = (s ^ MULTIPLIER) & MASK;
    }

    /** Advance and return the top 'bits' like Random.next(int). bits <= 32. */
    public int next(int bits) {
        seed = (seed * MULTIPLIER + ADDEND) & MASK;
        return (int)(seed >>> (48 - bits));
    }

    /** Advance once and return the full 48-bit state (true 48-bit output). */
    public long next48() {
        seed = (seed * MULTIPLIER + ADDEND) & MASK;
        return seed; // already 48-bit masked
    }

    public void printProperties() {
        System.out.println("Class: " + this.getClass().getName());
        System.out.printf(Locale.ROOT, "  %s = 0x%X (%d)%n", "multiplier", MULTIPLIER, MULTIPLIER);
        System.out.printf(Locale.ROOT, "  %s = 0x%X (%d)%n", "addend",     ADDEND,     ADDEND);
        System.out.printf(Locale.ROOT, "  %s = 0x%X (%d)%n", "mask",       MASK,       MASK);
    }

    public static void main(String[] args) {
        final int N = 5;
        final long SEED = 1L;
        final boolean OutputOnly32bits = true;
        final long OutputOnly32bitsShift = OutputOnly32bits ? (48L - 32L) : 0L;

        Lcg rng = new Lcg(SEED);

        rng.printProperties();
        System.out.println("Generating " + N + " 48-bit values from manual LCG:");

        for (int i = 0; i < N; i++) {
            long high = rng.next48() >>> OutputOnly32bitsShift;
            long low = rng.next48() >>> OutputOnly32bitsShift;

            if (OutputOnly32bits) {
                System.out.printf("%08X %08X%n", high, low);
            } else {
                System.out.printf("%012X %012X%n", high, low);
            }
        }
    }
}
