import java.util.Locale;

public final class ManualGen {
    // Java's Random LCG parameters
    private static final long MULTIPLIER = 0x5DEECE66DL;
    private static final long ADDEND     = 0xBL;
    private static final long MASK       = (1L << 48) - 1;

    // Internal 48-bit state
    private long seed;

    public ManualGen(long seed) {
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
        final String delimiter = "################################";
        final int N = 6;
        final long SEED = 1L;
        final boolean OutputOnly32bits = true;
        final long OutputOnly32bitsShift = OutputOnly32bits ? (48L - 32L) : 0L;
        final boolean emulateBallyMoneyHoney = true;
        long rouletteSlots = 32;

        ManualGen rng = new ManualGen(SEED);

        // rng.printProperties();
        System.out.println("Generating " + N + " 48-bit values from manual LCG:");
        System.out.println(delimiter);

        for (int i = 0; i < N; i++) {
            long first = rng.next48();
            long second = rng.next48();
            long third = rng.next48();

            if (i == N/2) {
                System.out.println(delimiter);
            }

            if (emulateBallyMoneyHoney) {
                // Emulate behavior of nextInt(rouletteSlots) on 48-bit state
                // Not exactly the same. Do not treat bias when not power of 2.
                first = first >> (48 - 31);
                second  = second  >> (48 - 31);
                third = third  >> (48 - 31);
                if ((rouletteSlots & (rouletteSlots - 1)) == 0) {  // is it power of 2?
                    // take top bits
                    first = (first * rouletteSlots) >>> 31;
                    second  = (second  * rouletteSlots) >>> 31;
                    third = (third * rouletteSlots) >>> 31;
                } else {
                    first = first % rouletteSlots;
                    second  = second  % rouletteSlots;
                    third = third % rouletteSlots;
                }
            } else {
                first = first >>> OutputOnly32bitsShift;
                second  = second  >>> OutputOnly32bitsShift;
                third = third >>> OutputOnly32bitsShift;
            }

            if (emulateBallyMoneyHoney) {
                System.out.printf("%02X %02X %02X%n", first, second, third);
            } else {
                if (OutputOnly32bits) {
                    System.out.printf("%08X %08X %08X%n", first, second, third);
                } else {
                    System.out.printf("%012X %012X %012X%n", first, second, third);
                }
            }
        }
    }
}
