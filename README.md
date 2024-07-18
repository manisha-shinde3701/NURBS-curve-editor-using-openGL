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
<!--     <td>
      <img src="https://github.com/user-attachments/assets/YOUR_IMAGE_ID_HERE" alt="Project Overview" width="300">
    </td> -->
    <td>
      <h3>My Project: Exploring Curve Representations</h3>
      <h4>Bezier Curves</h4>
      <p>Implemented a Bezier curve algorithm to generate smooth curves from a set of control points. Explored how adjusting the control points affects the shape of the curve.</p>
      <h4>B-Splines</h4>
      <p>Developed a B-spline curve implementation, demonstrating how local control allows for efficient editing and manipulation of complex shapes.</p>
      <h4>NURBS</h4>
      <p>Implemented a NURBS curve algorithm, showcasing its ability to represent both analytical and freeform shapes with high accuracy.</p>
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
<h1 align="center">Industrial Applications</h1>

<table>
  <tr>
    <td>
      <h3>1️⃣ Product Design</h3>
      <p>Bezier, B-spline, and NURBS curves are extensively used in CAD software to design the smooth, organic shapes of products like cars, electronics, and consumer goods.</p>
    </td>
    <td>
      <h3>2️⃣ Animation and Visual Effects</h3>
      <p>These curve representations are crucial for creating fluid, natural-looking motion and deformations of 3D models in animation and visual effects.</p>
    </td>
  </tr>
  <tr>
    <td>
      <h3>3️⃣ Architectural Modeling</h3>
      <p>Curved surfaces created with these techniques are important for designing innovative, sculptural architectural forms.</p>
    </td>
    <td>
      <h3>4️⃣ Medical Imaging</h3>
      <p>NURBS are used to accurately model complex anatomical structures from medical scans for diagnosis and treatment planning.</p>
    </td>
  </tr>
</table>
<h1 align="center">
  <span style="color: #4A90E2; font-family: Arial, sans-serif; font-size: 36px;">
    Achievements and Insights
  </span>
</h1>

<table>
  <tr>
    <td>
      <h3>Understanding Curve Representations</h3>
      <p>Through this project, I gained a deep understanding of the mathematical foundations and practical applications of Bezier curves, B-splines, and NURBS.</p>
    </td>
    <td>
      <h3>Importance in CAD/CAM</h3>
      <p>I recognized the crucial role these curve representations play in the CAD/CAM industry, enabling the design of complex, organic shapes that are essential for modern product design and manufacturing.</p>
    </td>
  </tr>
  <tr>
    <td>
      <h3>Opportunities for Innovation</h3>
      <p>Exploring these curve techniques inspired me to consider how they could be further developed and applied to create even more advanced design and engineering tools.</p>
    </td>
    <td>
      <h3>Practical Implementation</h3>
      <p>By implementing the algorithms myself, I gained hands-on experience in working with these mathematical models and a deeper appreciation for their computational complexity.</p>
    </td>
  </tr>
</table>

<table>
  <tr>
    <td>
      <img src="https://github.com/user-attachments/assets/42f70441-c747-43dd-8656-e3a3b7e2d196" alt="Conclusion" width="300">
    </td>
    <td>
      <h2>Conclusion</h2>
      <h3>1. Powerful Tools</h3>
      <p>Bezier curves, B-splines, and NURBS are powerful tools that enable the design and creation of complex, organic shapes that are essential in modern product design, engineering, and visual effects.</p>
      <h3>2. Continuous Evolution</h3>
      <p>As technology continues to advance, these curve representations will likely evolve and be integrated into even more sophisticated design and engineering workflows.</p>
      <h3>3. Exciting Future</h3>
      <p>The future of curve-based design holds great promise, with the potential for even more innovative applications and breakthroughs in various industries.</p>
    </td>
  </tr>
</table>










