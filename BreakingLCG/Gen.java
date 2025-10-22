import java.util.Random;
import java.lang.reflect.Field;

public class Gen extends Random {
    public long next48() {
        // access protected next(int bits)
        final long MASK = (1L << 48) - 1;
        return ((long) super.next(48)) & MASK;
    }

    public void printProperties() throws Exception {
        System.out.println("Class: " + this.getClass().getName());

        Class<?> superClass = Random.class;
        for (String name : new String[]{"multiplier", "addend", "mask"}) {
            Field field = superClass.getDeclaredField(name);
            field.setAccessible(true);
            long value = field.getLong(null);
            System.out.printf("  %s = 0x%X (%d)%n", name, value, value);
        }
    }

    public static void main(String[] args) {
        final int N = 5;
        final Integer SEED = 1;
        Gen rng = new Gen();

        if (SEED != 0) {
            rng.setSeed(SEED.intValue());
        }
        try {
            rng.printProperties();
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("Generating " + N + " 48-bit values from Random:");
        for (int i = 0; i < N; i++) {
            final long value = rng.next48();
            System.out.printf("0x%012x (%15d)%n", value, value);
        }
    }
}
