//-------------------------------------------------------------------------------------
package xpu.sw.tools.sdk.gui.components.menu.file.preferences.sections.editor;

//-------------------------------------------------------------------------------------
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.tree.*;

import org.apache.logging.log4j.*;

import xpu.sw.tools.sdk.common.context.*;

import xpu.sw.tools.sdk.gui.*;
import xpu.sw.tools.sdk.gui.components.menu.file.preferences.*;

//-------------------------------------------------------------------------------------

/**
 *
 * @author marius
 */
public class Editor extends javax.swing.JPanel {
    private Gui gui;
    private Context context;
    private Preferences preferences;
    
    /**
     * Creates new form Editor
     */
    public Editor(Gui _gui, Context _context, Preferences _preferences) {
        gui = _gui;
        context = _context;
        preferences = _preferences;
        initComponents();
        init();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jComboBox1 = new javax.swing.JComboBox<>();
        jLabel1 = new javax.swing.JLabel();
        jCheckBox1 = new javax.swing.JCheckBox();

        jComboBox1.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                jComboBox1ItemStateChanged(evt);
            }
        });

        jLabel1.setText("Theme:");

        jCheckBox1.setText("Reopen tabs at start");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(14, 14, 14)
                        .addComponent(jLabel1)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(jCheckBox1)))
                .addContainerGap(446, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jComboBox1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(jCheckBox1)
                .addContainerGap(293, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void jComboBox1ItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_jComboBox1ItemStateChanged
        // TODO add your handling code here:
        String _themeName = getSelectedTheme();
        gui.getMyComponents().getEditor().setTheme(_themeName);
    }//GEN-LAST:event_jComboBox1ItemStateChanged


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JCheckBox jCheckBox1;
    private javax.swing.JComboBox<String> jComboBox1;
    private javax.swing.JLabel jLabel1;
    // End of variables declaration//GEN-END:variables

//-------------------------------------------------------------------------------------
    private void init(){
        String _theme = context.getSdkConfig().getString("editor_theme", "Dark");
        java.util.List<String> _themes = gui.getServices().getUtils().getAvailableThemes();
        if(_themes != null){
            for(int i = 0; i < _themes.size();i++) {
                jComboBox1.addItem(_themes.get(i));
            }
            if((_theme != null) && (!_theme.isEmpty())){
                jComboBox1.setSelectedItem(_theme);            
            }
        }
    }

//-------------------------------------------------------------------------------------
    public String getSelectedTheme(){
        return (String)jComboBox1.getSelectedItem();
    }

//-------------------------------------------------------------------------------------
    public void save(){
        String _theme = getSelectedTheme();
        gui.getMyComponents().getEditor().setTheme(_theme);
        context.getSdkConfig().setProperty("editor_theme", _theme);
    }
    
//-------------------------------------------------------------------------------------
    public void apply(){
        String _theme = getSelectedTheme();
        gui.getMyComponents().getEditor().setTheme(_theme);
    }
    
//-------------------------------------------------------------------------------------
}
//-------------------------------------------------------------------------------------
