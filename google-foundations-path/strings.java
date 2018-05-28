public class strings 
{
    public static void main( String[] args )
    {
        String msg = "hello world";
        msg = msg.concat(" lovely day");
        msg = msg.toUpperCase();
        msg = msg.replace('o', 'p');

        char[] chars = msg.toCharArray();
        for(char c: chars){
            System.out.println(c);
        }
    }
}