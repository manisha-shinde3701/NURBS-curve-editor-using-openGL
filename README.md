# NURBS-curve-editor-using-openGL

 Bezier curves, B-splines, and NURBS are powerful mathematical tools used in computer graphics and CAD/CAM to create and manipulate smooth, complex curves and surfaces. These 


![image](https://github.com/user-attachments/assets/2f747776-a307-40bc-94dc-25755e5640fd)
They used something called a spline.
A spline was a long, flexible piece of plastic or wood with a rectangular cross-section. The spline was held in place by grounding it against a drawing board with heavy lead weights that had protrusions called ducks. The spline would then mold itself based on where the ducks kept it fixed. Moving the ducks meant altering the shape of the spline.
![image](https://github.com/user-attachments/assets/41e703ab-635f-4ecd-9506-2957ad6b4d46)
![image](https://github.com/user-attachments/assets/96425ed2-335a-4292-b7bc-4a707b9bb072)
<table>
  <tr>
    <th>Bezier Curve</th>
    <th>B-spline</th>
    <th>NURBS</th>
  </tr>
  <tr>
    <td>
<!--       <img src="https://github.com/user-attachments/assets/613ad060-7c12-4f5b-a262-34096e1a513b" alt="Bezier Curve"> -->
      <p>A Bezier curve is defined by a set of control points. The curve passes through the first and last points and is influenced by the intermediate control points, creating smooth, intuitive curves.</p>
    </td>
    <td>
<!--       <img src="https://github.com/user-attachments/assets/1d4b0771-10fb-4e52-bcf6-3d262e58f695" alt="B-spline"> -->
      <p>A B-spline is a piecewise polynomial curve defined by a set of control points. It provides local control, meaning changes to the control points only affect the nearby section of the curve.</p>
    </td>
    <td>
<!--       <img src="https://github.com/user-attachments/assets/fe5b446f-4ab5-4722-8a05-1baa0de34c82" alt="NURBS"> -->
      <p>NURBS (Non-Uniform Rational B-Splines) are a generalization of B-splines that can represent both analytical (e.g., circles) and freeform shapes with a high degree of accuracy.</p>
    </td>
  </tr>
</table>
<table>
  <tr>
    <table>
  <tr>
    <td>
      <img src="https://github.com/user-attachments/assets/46e590db-05ba-41d9-bdaa-f8bb4e4b3cc7" alt="Linear Curves" width="300">
    </td>
    <td>
      <h3>Linear curves</h3>
      <p>Let <em>t</em> denote the fraction of progress (from 0 to 1) the point <em>B(t)</em> has made along its traversal from <em>P<sub>0</sub></em> to <em>P<sub>1</sub></em>. For example, when <em>t=0.25</em>, <em>B(t)</em> is one quarter of the way from point <em>P<sub>0</sub></em> to <em>P<sub>1</sub></em>. As <em>t</em> varies from 0 to 1, <em>B(t)</em> draws a line from <em>P<sub>0</sub></em> to <em>P<sub>1</sub></em>.</p>
      <p><em>B(t) = P<sub>0</sub> + t(P<sub>1</sub> - P<sub>0</sub>) = (1 - t)P<sub>0</sub> + tP<sub>1</sub>, 0 &le; t &le; 1</em></p>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td>
      <img src="https://github.com/user-attachments/assets/5a349f9b-01f5-4bef-8dcf-0f1ac5bfa24d" alt="Quadratic Curves" width="300">
    </td>
    <td>
      <h3>Quadratic curves</h3>
      <p>A quadratic Bézier curve is the path traced by the function <em>B(t)</em>, given points <em>P<sub>0</sub></em>, <em>P<sub>1</sub></em>, and <em>P<sub>2</sub></em>.</p>
      <p><em>B(t) = (1 - t)[(1 - t)<em>P<sub>0</sub></em> + t<em>P<sub>1</sub></em>] + t[(1 - t)<em>P<sub>1</sub></em> + t<em>P<sub>2</sub></em>], 0 &le; t &le; 1</em></p>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td>
      <img src="https://github.com/user-attachments/assets/84357202-6ce6-4385-9ca9-4f6e79b4533a" alt="Cubic Curves" width="300">
    </td>
    <td>
      <h3>Cubic curves</h3>
      <p>For cubic curves one can construct intermediate points Q0, Q1, and Q2 that describe linear Bézier curves, and points R0 and R1 that describe quadratic Bézier curves:</p>
      <p><em>linear = (1 - t) + t</em></p>
      <p><em>square = (1 - t)² + 2 · (1 - t) · t + t²</em></p>
      <p><em>cubic = (1 - t)³ + 3 · (1 - t)² · t + 3 · (1 - t) · t² + t³</em></p>
    </td>
  </tr>
</table>

![image](https://github.com/user-attachments/assets/dd193998-ba33-486a-a8a4-5473bd3c1a20)









