import java.util.Scanner;
import javax.swing.JOptionPane;

class GUIMain {
	
	public static void main(String[] args) {

		String tiedosto;
		String temp;
		int maara;

		//kysytään tiedoston nimi ja arvaukset jonka jälkeen luodaan oliot ja graafinen käyttöliittymä

		tiedosto = JOptionPane.showInputDialog("Anna tiedoston nimi: ");
		temp = JOptionPane.showInputDialog("Anna arvausten maara: ");
		maara = Integer.parseInt(temp);

		Sanalista sanalista = new Sanalista(tiedosto);
		Hirsipuu peli = new Hirsipuu(sanalista.annaSanat(), maara);

		GUI ikkuna = new GUI(peli);
		
	}
}