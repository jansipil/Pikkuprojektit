import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.JOptionPane;

//luokka rakentaa graafisen käyttöliittymän
class GUI extends Frame implements ActionListener, WindowListener{

	private Label viestiLabel;
	private Label jaljellaLabel;
	private Label ohjeetLabel;
	private Label sanaLabel;
	private TextField arvausField;
	private TextField oikeinField;
	private Button arvausButton;

	private Hirsipuu tiedot;

	//asetetaan tarvittavat laatikot ja nappula konstruktorissa
	public GUI(Hirsipuu tarv_tiedot) {

		tiedot = tarv_tiedot;

		setLayout(new FlowLayout());
 
		viestiLabel = new Label("Anna kirjain");
		jaljellaLabel = new Label("Arvauksia jaljella: ");
		ohjeetLabel = new Label("Arvaa kaikki sanan kirjaimet voittaaksesi pelin");
		sanaLabel = new Label("Oikea sana on: " + tiedot.sana());
		arvausField = new TextField("", 5);
		oikeinField = new TextField("", 20);
		arvausButton = new Button("Arvaa");

		oikeinField.setEditable(false);

		add(viestiLabel); 
 		add(arvausField);

 		add(jaljellaLabel); 
 		add(oikeinField);

		add(arvausButton);
		arvausButton.addActionListener(this);

		add(ohjeetLabel);
		add(sanaLabel);

		addWindowListener(this);
 
		setTitle("Hirsipuu");  
		setSize(300, 300);

		setLocationRelativeTo(null);
		setVisible(true);
	}

	
	//metodi tekee tarvittavat asiat nappia painettaessa
	//tarkistaa onko kirjain oikein ja voitetaanko vai hävitäänkö peli
	public void actionPerformed(ActionEvent e) {

		char arvaus = arvausField.getText().charAt(0);
		boolean oikein = false;

		oikein = tiedot.arvaa(arvaus);

		if(oikein == true){
			oikeinField.setText("Kirjain oikein");
			arvausField.setText("");
		}
		else{
			oikeinField.setText("Kirjain vaarin");
			arvausField.setText("");
			jaljellaLabel.setText("Arvauksia jaljella: " + tiedot.arvauksiaOnJaljella());
		}

		if(tiedot.onLoppu() == true){
			JOptionPane.showMessageDialog( null, "Onnea, voitit pelin!");
			System.exit(0);
		}
		if(tiedot.arvauksiaOnJaljella() == 0){
			JOptionPane.showMessageDialog( null, "Havisit pelin!");
			System.exit(0);
		}

	}
	
	//metodi ja overridet mahdollistavat ohjelman sulkemisen punaisesta ruksista
	@Override
	public void windowClosing(WindowEvent e) {
		System.exit(0);
	}
	@Override public void windowOpened(WindowEvent e) { }
	@Override public void windowClosed(WindowEvent e) { }
	@Override public void windowIconified(WindowEvent e) { }
	@Override public void windowDeiconified(WindowEvent e) { }
	@Override public void windowActivated(WindowEvent e) { }
	@Override public void windowDeactivated(WindowEvent e) { }

}