/*
 * This file is part of JGAP.
 *
 * JGAP offers a dual license model containing the LGPL as well as the MPL.
 *
 * For licensing information please see the file license.txt included with JGAP
 * or have a look at the top of class org.jgap.Chromosome which representatively
 * includes the JGAP license policy applicable for any file delivered with JGAP.
 */
package examples.gp.symbolicRegression;

/*
  hakank: This is my take of the Sigmoid function.

  For more about Sigmoid function, see
  http://en.wikipedia.org/wiki/Sigmoid_function
    P(t) = 1/ (1+exp(-t))

  I just copied the org.jgap.gp.function.Log and changed
  accordingly...

 */

import org.jgap.*;
import org.jgap.gp.*;
import org.jgap.gp.impl.*;
import org.jgap.util.*;

/**
 * Returns the Sigmoid of a double value.
 *
 * @author Klaus Meffert
 * @since 3.5
 */
public class Sigmoid
    extends MathCommand implements ICloneable {
  /** String containing the CVS revision. Read out via reflection!*/
  private final static String CVS_REVISION = "$Revision: 1.1 $";

  public Sigmoid(final GPConfiguration a_conf, Class a_returnType)
      throws InvalidConfigurationException {
    super(a_conf, 1, a_returnType);
  }

  public String toString() {
    return "sigmoid &1";
  }

  /**
   * @return textual name of this command
   *
   * @author Hakan Kjellerstrand (based on Klaus Meffert's Log file)
   */
  public String getName() {
    return "Sigmoid";
  }

  public float execute_float(ProgramChromosome c, int n, Object[] args) {
    float f = c.execute_float(n, 0, args);
    return (float) 1.0f / (1.0f + (float) Math.exp( -f));
  }

  public double execute_double(ProgramChromosome c, int n, Object[] args) {
    double d = c.execute_double(n, 0, args);
    return 1.0d / (1.0d + Math.exp( -d));
  }

  public Object execute_object(ProgramChromosome c, int n, Object[] args) {
    return ( (Compatible) c.execute_object(n, 0, args)).execute_sigmoid();
  }

  protected interface Compatible {
    public Object execute_sigmoid();
  }
  /**
   * Clones the object. Simple and straight forward implementation here.
   *
   * @return cloned instance of this object
   *
   * @author Hakan Kjellerstrand (based on Klaus Meffert's Log file)
   */
  public Object clone() {
    try {
      Sigmoid result = new Sigmoid(getGPConfiguration(), getReturnType());
      return result;
    } catch (Exception ex) {
      throw new CloneException(ex);
    }
  }
}
