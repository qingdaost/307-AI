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
  hakank: This is my take of the Gamma function.

  The definition is from Eureqa: http://ccsl.mae.cornell.edu/eureqa_ops :
  - gamma function: gamma(x) = pow((x/e)*sqrt(x*sinh(1/x)), x)*sqrt(2pi/x)

  I just copied the org.jgap.gp.function.Log and changed
  accordingly...

 */


import org.jgap.*;
import org.jgap.gp.*;
import org.jgap.gp.impl.*;
import org.jgap.util.*;

/**
 * Returns the natural logarithm (base e) of a double value.
 *
 * @author Klaus Meffert
 * @since 3.5
 */
public class Gamma
    extends MathCommand implements ICloneable {
  /** String containing the CVS revision. Read out via reflection!*/
  private final static String CVS_REVISION = "$Revision: 1.1 $";

  public Gamma(final GPConfiguration a_conf, Class a_returnType)
      throws InvalidConfigurationException {
    super(a_conf, 1, a_returnType);
  }

  public String toString() {
    return "gamma &1";
  }

  /**
   * @return textual name of this command
   *
   * @author Hakan Kjellerstrand (based on Klaus Meffert's Log file)
   */
  public String getName() {
    return "Gamma";
  }

  public float execute_float(ProgramChromosome c, int n, Object[] args) {
    float f = c.execute_float(n, 0, args);
    return (float) Math.pow( (f / Math.E) * Math.sqrt(f * Math.sinh(1.0f / f)),
                            f) * (float) Math.sqrt(2.0f * Math.PI / f);
  }

  public double execute_double(ProgramChromosome c, int n, Object[] args) {
    double d = c.execute_double(n, 0, args);
    return Math.pow( (d / Math.E) * Math.sqrt(d * Math.sinh(1.0 / d)), d) *
        Math.sqrt(2.0 * Math.PI / d);
  }

  public Object execute_object(ProgramChromosome c, int n, Object[] args) {
    return ( (Compatible) c.execute_object(n, 0, args)).execute_gamma();
  }

  protected interface Compatible {
    public Object execute_gamma();
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
      Gamma result = new Gamma(getGPConfiguration(), getReturnType());
      return result;
    } catch (Exception ex) {
      throw new CloneException(ex);
    }
  }
}
